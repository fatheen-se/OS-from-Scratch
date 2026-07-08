#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants for VGA text mode
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xb8000;

// Text mode colors
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// Terminal state
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

// Helper functions for VGA text mode
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

// Delay function (simple busy wait)
void delay(int count) {
    for(int i = 0; i < count * 10000000; i++) {
        __asm__("nop");
    }
}

// Initialize terminal
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

// Change terminal color
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Put character at position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

// Handle scrolling
void terminal_scroll() {
    // Move all lines up one
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
        }
    }
    
    // Clear bottom line
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
    }
}

// Put a character
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
        return;
    }
    
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
    }
}

// Write a string
void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

// Write a null-terminated string
void terminal_writestring(const char* data) {
    for (size_t i = 0; data[i]; i++)
        terminal_putchar(data[i]);
}

// Set cursor position
void terminal_set_cursor(int row, int col) {
    terminal_row = row;
    terminal_column = col;
}

// Clear the terminal
void terminal_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
}

// Draw MARSTECH logo using ASCII art
void draw_marstech_logo() {
    uint8_t logo_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    terminal_setcolor(logo_color);
    
    terminal_set_cursor(5, 15);
    terminal_writestring(" __  __    _    ____  ____ _____ _____ ____ _   _ ");
    terminal_set_cursor(6, 15);
    terminal_writestring("|  \\/  |  / \\  |  _ \\/ ___|_   _| ____/ ___| | | |");
    terminal_set_cursor(7, 15);
    terminal_writestring("| |\\/| | / _ \\ | |_) \\___ \\ | | |  _|| |   | |_| |");
    terminal_set_cursor(8, 15);
    terminal_writestring("| |  | |/ ___ \\|  _ < ___) || | | |__| |___|  _  |");
    terminal_set_cursor(9, 15);
    terminal_writestring("|_|  |_/_/   \\_\\_| \\_\\____/ |_| |_____\\____|_| |_|");
}

// Show welcome message with fade-in effect
void draw_welcome_message_with_fade() {
    const char* message = "WELCOME TO MARSTECH";
    int message_length = 19; // Length of message above
    int start_col = 25;
    int start_row = 13;
    
    // Create fade-in effect by gradually increasing color intensity
    for (int intensity = 0; intensity <= 15; intensity++) {
        // Set color with increasing intensity
        uint8_t color = vga_entry_color(intensity, VGA_COLOR_BLUE);
        
        // Draw the message at current intensity
        terminal_set_cursor(start_row, start_col);
        for (int i = 0; i < message_length; i++) {
            terminal_putentryat(message[i], color, start_col + i, start_row);
        }
        
        // Small delay between intensity changes
        delay(1);
    }
}

// Show Windows-like welcome screen
void show_welcome_screen() {
    // Set background to blue (Windows-like)
    uint8_t bg_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    
    // Fill screen with background color
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_putentryat(' ', bg_color, x, y);
        }
    }
    
    // Draw logo 
    draw_marstech_logo();
    
    // Draw welcome message with fade-in effect
    draw_welcome_message_with_fade();
    
    // Display for 8 seconds
    delay(80);
    
    // Return to normal screen
    terminal_clear();
}

void kernel_main(void) {
    // Initialize terminal
    terminal_initialize();
    
    // Show welcome screen
    show_welcome_screen();
    
    // After welcome screen, display regular terminal
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_clear();
    
    // Print welcome message
    terminal_writestring("\n\n                          WELCOME TO MARSTECH OS!\n\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_writestring("SUCCESSFULLY ENTERED THE LONG MODE.\n");
    
    // Demonstrate different text colors
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_writestring("Futher Development on the process\n");
    
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_writestring("                                  Complete OS Coming Soon\n\n");
    
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_writestring("Developer M.N.Fatheen\nProud to be the Student of MARSTECH\n\nSpecial Thanks to my lecturer RAMZEEN SIR for his guidance\nmotivation and support\n");
    
    // Return to normal color
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_writestring("\n                      MARS TECHNOLOGY SYSTEMS READY.\n");
}