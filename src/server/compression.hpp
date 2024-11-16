#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include <iostream>
#include <sstream>
#include <string>

namespace io = boost::iostreams;

std::string compress_string(const std::string &input) {
	std::stringstream compressed_stream;
	io::filtering_streambuf<io::output> out;

	// Add the zlib compression filter
	out.push(io::zlib_compressor());

	// Attach the compressed stream
	out.push(compressed_stream);

	// Write the input data to be compressed
	std::stringstream input_stream(input);
	boost::iostreams::copy(input_stream, out);

	// Get the compressed data as a string
	return compressed_stream.str();
}

std::string decompress_string(const std::string &input) {
	std::stringstream compressed_stream(input);
	std::stringstream decompressed_stream;

	io::filtering_streambuf<io::input> in;

	// Add the zlib decompression filter
	in.push(io::zlib_decompressor());

	// Attach the compressed stream
	in.push(compressed_stream);

	// Read and decompress
	boost::iostreams::copy(in, decompressed_stream);

	// Get the decompressed data as a string
	return decompressed_stream.str();
}

int main() {
	// Original string to compress
	std::string original_data =
		"This is a test string to compress using zlib with Boost.";

	// Compress the string
	std::string compressed_data = compress_string(original_data);
	std::cout << "Compressed Data Size: " << compressed_data.size()
			  << std::endl;

	// Decompress the string
	std::string decompressed_data = decompress_string(compressed_data);
	std::cout << "Decompressed Data: " << decompressed_data << std::endl;

	// Verify if decompressed data matches original
	if (original_data == decompressed_data) {
		std::cout << "Success: Decompressed data matches original!"
				  << std::endl;
	} else {
		std::cout << "Error: Decompressed data does not match original!"
				  << std::endl;
	}

	return 0;
}
