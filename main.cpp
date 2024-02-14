#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

std::vector<std::string> load_table(const std::string& file)
{
	std::ifstream in(file);
	std::vector<std::string> ws_table;
	while(!in.eof())
	{
		ws_table.push_back("");
		std::getline(in, ws_table.back());
	}
	return std::move(ws_table);
}

bool is_rectangular(const std::vector<std::string>& ws_table)
{
	for(std::size_t i = 1; i < ws_table.size(); ++i)
	{
		if(ws_table[i - 1].size() != ws_table[i].size())
		{
			return false;
		}
	}
	return true;
}

bool is_valid(const std::vector<std::string>& ws_table)
{
	return is_rectangular(ws_table) && ws_table.size() > 0 && ws_table[0].size() > 0;
}

enum class Direction {Up, Down, Left, Right, UpLeft, DownLeft, UpRight, DownRight, NotFound};

std::string to_string(Direction d)
{
	switch(d)
	{
	case Direction::Down:
		return "Down";
	case Direction::Up:
		return "Up";
	case Direction::Left:
		return "Left";
	case Direction::Right:
		return "Right";
	case Direction::UpLeft:
		return "Up-left";
	case Direction::DownLeft:
		return "Down-left";
	case Direction::UpRight:
		return "Up-right";
	case Direction::DownRight:
		return "Down-right";
	default:
		return "Not found";
	}
}

std::tuple<std::size_t, bool> find_in_line(const std::string& line, std::string text)
{
	std::size_t position = line.find(text);
	if(position < line.size())
	{
		return std::make_tuple(position, false);
	}
	std::reverse(text.begin(), text.end());
	return std::make_tuple(line.find(text), true);
}

std::tuple<std::size_t, std::size_t, Direction> find_in_table(const std::vector<std::string>& ws_table, const std::string& text)
{
	for(std::size_t y = 0; y < ws_table.size(); ++y)
	{
		auto [position, reverse] = find_in_line(ws_table[y], text);
		if(position < ws_table[y].size())
		{
			return std::make_tuple(y, position, reverse ? Direction::Left : Direction::Right);
		}
	}
	for(std::size_t x = 0; x < ws_table[0].size(); ++x)
	{
		std::string column;
		for(std::size_t y = 0; y < ws_table.size(); ++y)
		{
			column += ws_table[y][x];
		}
		auto [position, reverse] = find_in_line(column, text);
		if(position < column.size())
		{
			return std::make_tuple(position, x, reverse ? Direction::Up : Direction::Down);
		}
	}
	for(std::size_t y = 0; y < ws_table.size(); ++y)
	{
		std::string diagonal;
		for(std::size_t x = 0, iy = y; x < ws_table[0].size() && iy < ws_table.size(); ++x, ++iy)
		{
			diagonal += ws_table[iy][x];
		}
		auto [position, reverse] = find_in_line(diagonal, text);
		if(position < diagonal.size())
		{
			return std::make_tuple(y + position, position, reverse ? Direction::UpLeft : Direction::DownRight);
		}
	}
	for(std::size_t x = 0; x < ws_table[0].size(); ++x)
	{
		std::string diagonal;
		for(std::size_t ix = x, y = 0; ix < ws_table[0].size() && y < ws_table.size(); ++ix, ++y)
		{
			diagonal += ws_table[y][ix];
		}
		auto [position, reverse] = find_in_line(diagonal, text);
		if(position < diagonal.size())
		{
			return std::make_tuple(position, x + position, reverse ? Direction::UpLeft : Direction::DownRight);
		}
	}
	for(std::size_t y = 0; y < ws_table.size(); ++y)
	{
		std::string diagonal;
		for(std::size_t x = 0, iy = y + 1; x < ws_table[0].size() && iy > 0; ++x, --iy)
		{
			diagonal += ws_table[iy - 1][x];
		}
		auto [position, reverse] = find_in_line(diagonal, text);
		if(position < diagonal.size())
		{
			return std::make_tuple(y - position, position, reverse ? Direction::DownLeft : Direction::UpRight);
		}
	}
	for(std::size_t x = 0; x < ws_table[0].size(); ++x)
	{
		std::string diagonal;
		for(std::size_t ix = x, y = ws_table.size(); ix < ws_table[0].size() && y > 0; ++ix, --y)
		{
			diagonal += ws_table[y - 1][ix];
		}
		auto [position, reverse] = find_in_line(diagonal, text);
		if(position < diagonal.size())
		{
			return std::make_tuple(ws_table[0].size() - 1 - position, x + position, reverse ? Direction::DownLeft : Direction::UpRight);
		}
	}
	return std::make_tuple(std::size_t(0), std::size_t(0), Direction::NotFound);
}

void print(const std::vector<std::string>& ws_table)
{
	for(const auto& line : ws_table)
	{
		std::cout << line << '\n';
	}
}

int main()
{
	std::string input;
	std::cout << "Enter file name: ";
	std::getline(std::cin, input);
	auto ws_table = load_table(input);
	print(ws_table);
	if(!is_valid(ws_table))
	{
		std::cout << "Error: Table for word search must be a non-empty rectangle\n";
		return 0;
	}
	for(;;)
	{
		std::cout << "Enter searched word: ";
		std::getline(std::cin, input);
		if(input.empty())
		{
			break;
		}
		auto [y, x, direction] = find_in_table(ws_table, input);
		std::cout << "x: " << x << "\ny: " << y << "\ndirection: " << to_string(direction) << '\n';
	}
	return 0;
}
