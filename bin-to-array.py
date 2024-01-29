"""
Use this script to write data files as c arrays. This is used to stay cross-platform for embedding.
"""
import argparse
import os.path


def format_hex_line(data):
    hex_data = ', '.join(f'0x{byte:02x}' for byte in data)
    return f'    {hex_data},\n'


def hexdump_to_cpp_array(file_name: str, output_dir: str, variable_name: str):
    path = os.path.join(output_dir, variable_name)
    header_path, source_path = f"{path}.h", f"{path}.cpp"

    # Header
    with open(header_path, 'w') as file:
        file.write("#pragma once\n\n")
        file.write("#include <cstddef>\n\n")
        file.write(f"extern const unsigned char {variable_name}[];\n")
        file.write(f"extern const size_t {variable_name}Length;\n")

    # Source
    byte_count = 0
    with open(file_name, 'rb') as bin_file, open(source_path, 'w') as source_file:
        source_file.write(f"// This is the bytes of {file_name}\n")
        source_file.write(f'#include "{variable_name}.h"\n\n')
        source_file.write(f"const unsigned char {variable_name}[] = {{\n")
        while True:
            chunk = bin_file.read(16)
            if not chunk:
                break
            byte_count += len(chunk)
            source_file.write(format_hex_line(chunk))
        source_file.write("};\n")
        source_file.write(f"const size_t {variable_name}Length = {byte_count};\n")


def main():
    parser = argparse.ArgumentParser(description="Hexdump a file to a cpp array.")
    parser.add_argument("file", help="Path to the binary file to be dumped")
    parser.add_argument("-d", "--dir", help="Output directory", metavar="OUTPUT")
    parser.add_argument("-n", "--name", help="Name of the array and header/source files", default="data")
    args = parser.parse_args()

    if args.dir:
        try:
            hexdump_to_cpp_array(args.file, args.dir, args.name)
            path = os.path.join(args.dir, args.name)
            print(f"Hex dump written to {path}.h & {path}.cpp")
        except FileNotFoundError:
            print(f"File not found: {args.file}")
        except IOError as e:
            print(f"Error: {e}")
    else:
        print("No output dir specified. Exiting.")


if __name__ == "__main__":
    main()
