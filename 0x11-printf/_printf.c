#include <stdarg.h>
#include <unistd.h>

typedef struct {
    char specifier;
    void (*function)(va_list *, int *);
} format_specifier;

void print_char(va_list *args, int *count) {
    char c = va_arg(*args, int);
    write(1, &c, 1);
    (*count)++;
}

void print_string(va_list *args, int *count) {
    char *str = va_arg(*args, char *);
    while (*str) {
        write(1, str, 1);
        str++;
        (*count)++;
    }
}

void print_percent(va_list *args, int *count) {
    write(1, "%", 1);
    (*count)++;
}

static format_specifier format_specifiers[] = {
    {'c', print_char},
    {'s', print_string},
    {'%', print_percent},
    {0, NULL}
};

int _printf(const char *format, ...) {
    int count = 0;
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            for (int i = 0; format_specifiers[i].specifier; ++i) {
                if (*format == format_specifiers[i].specifier) {
                    format_specifiers[i].function(&args, &count);
                    break;
                }
            }
            format++;
        } else {
            write(1, format, 1);
            format++;
            count++;
        }
    }

    va_end(args);
    return count;
}
