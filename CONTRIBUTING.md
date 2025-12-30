# Contributing to Device Trust Shim

Thank you for your interest in contributing to Device Trust Shim (DTS)!

## Development Setup

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15 or later
- Git

### Building from Source

```bash
git clone https://github.com/BigDataPlumbing/device_trust_shim.git
cd device_trust_shim
mkdir build && cd build
cmake ..
make
```

### Running Tests

```bash
cd build
ctest --output-on-failure
```

## Code Style

- Follow C++17 best practices
- Use meaningful variable and function names
- Add comments for non-obvious logic
- Keep functions focused and small
- Prefer `const` where possible

## Submission Process

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass (`ctest`)
6. Commit your changes (`git commit -m 'Add amazing feature'`)
7. Push to the branch (`git push origin feature/amazing-feature`)
8. Open a Pull Request

## Areas for Contribution

- **Performance Optimization**: Reduce memory footprint or CPU usage
- **Additional Hash Algorithms**: Support for SHA-3, BLAKE3, etc.
- **Log Compression**: Add compression support for storage efficiency
- **Multi-threading**: Thread-safe logging for multi-core devices
- **Hardware Integration**: HSM-backed key storage support
- **Documentation**: Improve examples, add integration guides
- **Testing**: Expand test coverage, add fuzzing

## Questions?

Open an issue on GitHub or contact [Big Data Plumbing](https://www.bigdataplumbing.com).

