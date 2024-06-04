#include <stdexcept>
#include <iostream>
#include <sstream>
#include <numeric>
#include <iomanip>
#include <bitset>
#include <string>
#include <vector>

std::string text2bin(int dec, int bits = 8)
{
  if (bits <= 0 || bits >= 32)
  {
    throw std::invalid_argument("Invalid number of bits");
  }

  std::string binary(bits, '0');

  for (int i = 0; i < bits; ++i)
  {
    if (dec & ( 1 << bits - 1 - i))
    {
      binary[i] = '1';
    }
  }

  return binary;
}

std::string text2bin(const std::string& text)
{
  std::stringstream binary;

  for (unsigned char c : text)
  {
    if (c < 0x80)
    {
      binary << text2bin(c, 8) << " ";
    }
    
    else if (c < 0xC0)
    {
      throw std::invalid_argument("Invalid UTF-8 encoding");
    }

    else if (c < 0xE0)
    {
      unsigned char fbyte = c;
      unsigned char sbyte = static_cast<unsigned char>(text[++c]);

      if ((sbyte & 0xC0) != 0x80)
      {
        throw std::invalid_argument("Invalid UTF-8 encoding");
      }

      binary << text2bin(fbyte, 8) << " " << text2bin(sbyte, 8) << " ";
    }

    else if (c < 0xF0)
    {
      unsigned char fbyte = c;
      unsigned char sbyte = static_cast<unsigned char>(text[++c]);
      unsigned char tbyte = static_cast<unsigned char>(text[++c]);

      if ((sbyte & 0xC0) != 0x80 || (tbyte & 0xC0) != 0x80)
      {
        throw std::invalid_argument("Invalid UTF-8 encoding");
      }

      binary << text2bin(fbyte, 8) << " " << text2bin(sbyte, 8) << " " << text2bin(tbyte, 8) << " ";
    }

    else if (c < 0xF8)
    {
      unsigned char fbyte = c;
      unsigned char sbyte = static_cast<unsigned char>(text[++c]);
      unsigned char tbyte = static_cast<unsigned char>(text[++c]);
      unsigned char fobyte = static_cast<unsigned char>(text[++c]);

      if ((sbyte & 0xC0) != 0x80 || (tbyte & 0xC0) != 0x80 || (fobyte & 0xC0) != 0x80)
      {
        throw std::invalid_argument("Invalid UTF-8 encoding");
      }

      binary << text2bin(fbyte, 8) << " " << text2bin(sbyte, 8) << " " << text2bin(tbyte, 8) << " " << text2bin(fobyte, 8) << " ";
    }
  }

  return binary.str();
}

int main()
{
  try
  {
    std::string text = "Goodbye World";

    std::string binary = text2bin(text);

    std::cout << "Binary: " << binary << std::endl;
  }

  catch (const std::invalid_argument& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}