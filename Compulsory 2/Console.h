#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

namespace console
{
    class ModifierException final : public std::exception
    {
        const char* what() const noexcept override
        {
            return "Custom color requires Table or RGB values";
        }
    };

    enum Code
    {
        FG_BLACK = 30,
        FG_RED = 31,
        FG_GREEN = 32,
        FG_ORANGE = 33,
        FG_BLUE = 34,
        FG_MAGENTA = 35,
        FG_CYAN = 36,
        FG_WHITE = 37,
        FG_CUSTOM = 38,
        FG_DEFAULT = 39,
        BG_BLACK = 40,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_ORANGE = 43,
        BG_BLUE = 44,
        BG_MAGENTA = 45,
        BG_CYAN = 46,
        BG_WHITE = 47,
        BG_CUSTOM = 48,
        BG_DEFAULT = 49,
        FG_BRIGHT_BLACK = 90,
        FG_BRIGHT_RED = 91,
        FG_BRIGHT_GREEN = 92,
        FG_BRIGHT_ORANGE = 93,
        FG_BRIGHT_BLUE = 94,
        FG_BRIGHT_MAGENTA = 95,
        FG_BRIGHT_CYAN = 96,
        FG_BRIGHT_WHITE = 97,
        BG_BRIGHT_BLACK = 100,
        BG_BRIGHT_RED = 101,
        BG_BRIGHT_GREEN = 102,
        BG_BRIGHT_ORANGE = 103,
        BG_BRIGHT_BLUE = 104,
        BG_BRIGHT_MAGENTA = 105,
        BG_BRIGHT_CYAN = 106,
        BG_BRIGHT_WHITE = 107
    };

    class Modifier
    {
        Code code;
        byte r, g, b;
        bool bit24;

    public:
        /**
         * @brief Makes a console color modifier
         *
         * @param p_code The modifier mode
         * @return The modifier to use with: cout << modifier << "text";
         */
        explicit Modifier(const Code p_code) : code(p_code), r(0), g(0), b(0), bit24(false)
        {
            if (p_code == FG_CUSTOM || p_code == BG_CUSTOM)
            {
                throw ModifierException();
            }
        }

        /**
         * @brief Makes a console color modifier with a custom color or table color
         *
         * 8 bit colors are table lookups, 24 bit colors are RGB values
         *
         * @param p_code The modifier mode
         * @param table_or_r_color The table color or red value
         * @param g The green value
         * @param b The blue value
         * @param bit24 Whether the color is a 24-bit color or 8 bit value
         * @return The modifier to use with: cout << modifier << "text";
         */
        Modifier(const Code p_code, const byte table_or_r_color, const byte g = 0, const byte b = 0, const bool bit24 = false) : code(p_code), r(table_or_r_color), g(g), b(b), bit24(bit24) {}
        friend std::ostream&
            operator<<(std::ostream& os, const Modifier& mod)
        {
            if (mod.code != FG_CUSTOM && mod.code != BG_CUSTOM)
                return os << "\033[" << mod.code << "m";
            if (mod.bit24)
                return os << "\033[" << mod.code << ";2;" << mod.r << ";" << mod.g << ";" << mod.b << "m";
            return os << "\033[" << mod.code << ";5;" << mod.r << "m";
        }
    };

    template <typename T>
    void print_element(T t, const int& width, bool right_align = false, Modifier modifier = Modifier(FG_DEFAULT), char separator = ' ')
    {
        if (right_align)
            std::cout << modifier << std::right << std::setw(width) << std::setfill(separator) << t << Modifier(FG_DEFAULT) << Modifier(BG_DEFAULT);
        else
            std::cout << modifier << std::left << std::setw(width) << std::setfill(separator) << t << Modifier(FG_DEFAULT) << Modifier(BG_DEFAULT);
    }

    int nameLen = 0;
    int minLen = 0;
    int maxLen = 0;
    int sdLen = 0;

    class TimeFormat
    {
    private:
        static std::string format_number(long long num)
        {
            std::string result = std::to_string(num);
            int i = result.length() - 3;
            while (i > 0)
            {
                result.insert(i, ",");
                i -= 3;
            }
            return result;
        }

        static std::string format_double(double num)
        {
            return std::to_string(num / 1000 - static_cast<long long>(num / 1000));
        }

        static std::string format_time(double time)
        {
            auto add = std::to_string(time / 1000 - static_cast<long long>(time / 1000));
            add = add.substr(add.find('.') + 1, 3);
            add = add.substr(0, add.find_last_not_of('0') + 1);
            if (!add.empty())
                add = "." + add;
            return format_number(static_cast<long long>(time / 1000)) + add + "ms";
        }

    public:
        /**
         * \brief Formats the time to ms and prints it as a part of the table
         * \param name The name for the table row
         * \param time The times in min,max,sd
         * \param dry If it should just set the size of the string length instead of printing it
         */
        static void print_time(std::string name, std::vector<double> time, bool dry = false)
        {
            const auto minStr = format_time(time[0]);
            const auto maxStr = format_time(time[1]);
            const auto sdStr = format_time(time[2]);
            if (!dry)
            {
                print_element(name, nameLen);
                std::cout << u8" │ ";
                print_element(minStr, minLen, true, Modifier(FG_BRIGHT_CYAN));
                std::cout << u8" │ ";
                print_element(maxStr, maxLen, true, Modifier(FG_BRIGHT_ORANGE));
                std::cout << u8" │ ";
                print_element(sdStr, sdLen, true, Modifier(FG_BRIGHT_MAGENTA));
                std::cout << std::endl;
            }
            else
            {
                minLen = max(minLen, static_cast<int>(minStr.length()));
                maxLen = max(maxLen, static_cast<int>(maxStr.length()));
                sdLen = max(sdLen, static_cast<int>(sdStr.length()));
                nameLen = max(nameLen, static_cast<int>(name.length()));
            }
        }

        /**
         * \brief Prints the table header
         */
        static void print_header()
        {
            print_element("Name", nameLen);
            std::cout << u8" │ ";
            print_element("Min", minLen, true, Modifier(FG_DEFAULT));
            std::cout << u8" │ ";
            print_element("Max", maxLen, true, Modifier(FG_DEFAULT));
            std::cout << u8" │ ";
            print_element("SD", sdLen, true, Modifier(FG_DEFAULT));
            std::cout << std::endl;
        }

        /**
         * \brief Prints the table separator
         */
        static void print_separator()
        {
            constexpr auto c = u8"─";
            for (int i = 0; i < nameLen + 1; ++i)
            {
                std::cout << c;
            }
            std::cout << u8"┼";
            for (int i = 0; i < minLen + 2; ++i)
            {
                std::cout << c;
            }
            std::cout << u8"┼";
            for (int i = 0; i < maxLen + 2; ++i)
            {
                std::cout << c;
            }
            std::cout << u8"┼";
            for (int i = 0; i < sdLen + 2; ++i)
            {
                std::cout << c;
            }
            std::cout << std::endl;
        }
    };
}
