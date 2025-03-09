#include "Engine.h"

// Removes all non-alphabet characters from a string
std::string removeSpecialCharacter(std::string s)  
{
	for (int i = 0; i < s.size(); i++) {

		if (s[i] < 'A' || s[i] > 'Z' && s[i] < 'a'
			|| s[i] > 'z') {
			s.erase(i, 1);
			i--;
		}
	}
	
	return s;

}


// Replaces J with I
std::string Engine::alterString(std::string str)  
{
	std::replace(str.begin(), str.end(), 'J', 'I');
	std::replace(str.begin(), str.end(), 'j', 'i');
	return str;
}



// Creates the encryption table using a keyword
std::vector<std::vector<char>> Engine::createTable(std::string keyword)  
{
	std::vector<std::vector<char>> playfairTable(5, std::vector<char>(5, '\0'));

	// Combine keyword with remaining alphabet (excluding J)
	std::string keyString = keyword + "ABCDEFGHIKLMNOPQRSTUVWXYZ";
	std::transform(keyString.begin(), keyString.end(), keyString.begin(), ::toupper);  //keyString is transformed to the upper case

	

	for (int k = 0; k < keyString.length(); k++)
	{
		bool repeated = false;
		bool used = false;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (playfairTable[i][j] == (keyString[k]))
				{
					repeated = true;
				}
				else if ((playfairTable[i][j] == '\0') && !repeated && !used)
				{
					playfairTable[i][j] =  keyString[k];
					used = true;
					break;
				}
			}
		}
	}

	return  playfairTable;
}




// Prepares text for encryption by handling duplicates and padding
std::string Engine::encipher(std::string text, char separator)
{
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);

	int digraphQuantity = (int)text.length() / 2 + text.length() % 2;

	for (int i = 0; i < (digraphQuantity - 1); i++)
	{
		// Insert separator between identical consecutive characters
		if (text[2 * i] == text[2 * i + 1])
		{

			text.insert((text.begin() + 2 * i + 1), separator);
			digraphQuantity = (int)text.length() / 2 + text.length() % 2;
		}
		
	}

	// Add separator in the end if length is odd
	if (!((text.length() % 2) == 0))
	{
		text = text + separator;
	}


	std::vector<std::string> digraphs;

	for (int j = 0; j < digraphQuantity; j++)
	{
		digraphs.push_back("");
	}

	
	for (int j = 0; j < digraphQuantity; j++)
	{
		int first = 2 * j;
		int second = 2 * j + 1;
		
		digraphs[j] = text[first];
		digraphs[j]+=text[second];
		
	}

	std::vector<std::string> encDigraphs = encodeDigraph(digraphs);

	std::string out = "";

	for (int i = 0; i < encDigraphs.size(); i++)
	{
		out += encDigraphs[i];
		
	}

	
	return out;
}


// Encrypts a vector of digraphs using Playfair rules
std::vector<std::string> Engine::encodeDigraph(std::vector<std::string> digraphs)
{
	std::vector<std::string> enc;

	for (std::string element : digraphs)
	{
	
		char a = element[0];
		char b = element[1];
		
		int row1 = getPoint(a).x;
		int row2 = getPoint(b).x;

		int column1 = getPoint(a).y;
		int column2 = getPoint(b).y;


		//Both letters in digraph are in the same row
		if (row1 == row2)
		{
			column1 = (column1 + 1) % 5;
			column2 = (column2 + 1) % 5;

		//Both letters in digraph are in the same column
		}
		else if (column1 == column2)
		{
			row1 = (row1 + 1) % 5;
			row2 = (row2 + 1) % 5;

		//Letters in digraph do not belong to the same row or column
		}
		else
		{
			int t = column1;
			column1 = column2;
			column2 = t;
		}
		std::string t1{ table[row1][column1] };
		std::string t2 = { table[row2][column2] };
		

		enc.push_back(t1 + t2);
	
	}


	return enc;
}

// Finds the position of a character in the Playfair table
Point Engine::getPoint(char c)
{
	Point pt(0, 0);
	
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (c == table[i][j])
			{
				pt.setX(i);
				pt.setY(j);
			}
		}
	}
	return pt;
}


// Main encryption function
std::string Engine::encode(std::string text, std::string keyword, char separator)
{

	std::vector<std::string> splittedInput1;
	std::vector<std::string> splittedInput;
	std::stringstream ss(text);
	std::string word;
	while (ss >> word)
	{
		splittedInput.push_back(word);
	}

	std::vector<std::vector<bool>> time;

	// Preprocess input text
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);

	std::stringstream ss1(text);
	while (ss1 >> word)
	{
		splittedInput1.push_back(word);
	}
	
	// Encrypt and combine results
	table = createTable(keyword);

	for (int i = 0; i < splittedInput.size(); i++)
	{
		
		splittedInput[i] = encipher(splittedInput[i], separator);
		
	}

	std::string output = "";

	for (int i = 0; i < splittedInput.size(); i++)
	{
		output += " " + splittedInput[i];

		
	}

	// Added to trim whitespace
	size_t pos = output.find(' ');
	if (pos != std::string::npos) {
		output.erase(pos, 1);
	}

	return output;
}


// Main decryption function
std::string Engine::decode(std::string text, std::string keyword, char separator)
{
	if (text.empty()) {
		throw std::invalid_argument("Input text cannot be empty.");
	}
	if (keyword.empty()) {
		throw std::invalid_argument("Keyword cannot be empty.");
	}
	if (separator == '\0') {
		throw std::invalid_argument("Separator cannot be null.");
	}

	// Initialize Playfair table
	table = createTable(keyword);
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);
	// Split the input text into chunks
	std::vector<std::string> splittedOut;
	std::stringstream ss(text);
	std::string word;
	while (ss >> word) {
		splittedOut.push_back(word);
	}

	// Decoding each chunk
	std::vector<std::string> decodedChunks;
	for (const auto& chunk : splittedOut) {
		std::string decodedChunk;
		for (size_t j = 0; j < chunk.length(); j += 2) {
			if (j + 1 >= chunk.length()) {
				throw std::out_of_range("Incomplete digraph found in the input text.");
			}

			char a = chunk[j];
			char b = chunk[j + 1];
			Point p1 = getPoint(a);
			Point p2 = getPoint(b);

			int r1 = p1.x, c1 = p1.y;
			int r2 = p2.x, c2 = p2.y;

			if (r1 == r2) {
				c1 = (c1 + 4) % 5;
				c2 = (c2 + 4) % 5;
			}
			else if (c1 == c2) {
				r1 = (r1 + 4) % 5;
				r2 = (r2 + 4) % 5;
			}
			else {
				std::swap(c1, c2);
			}

			decodedChunk += table[r1][c1];
			decodedChunk += table[r2][c2];
		}

		// Remove separator from the decoded chunk
		decodedChunk = removeSpecialCharacter(decodedChunk);
		decodedChunks.push_back(decodedChunk);
	}

	// Combine all decoded chunks into a single string
	std::string decoded;
	for (const auto& chunk : decodedChunks) {
		if (!decoded.empty()) {
			decoded += " ";
		}
		decoded += chunk;
	}

	return decoded;
}