# Changelog

All notable changes to Device Trust Shim (DTS) will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-01-15

### Added
- Initial release of Device Trust Shim (DTS)
- Header-only `AuditChain` class for tamper-evident audit logging
- SHA-256 hash implementation (lightweight, no external dependencies)
- Cryptographic chain mechanism preventing log deletion/modification
- JSON-formatted log output with embedded integrity hashes
- Chain verification API for integrity validation
- User ID and severity level enumeration support
- Example implementations for radiology devices and infusion pumps
- CMake build system with examples and tests
- Comprehensive README with API documentation
- Unit tests for core functionality

### Design Decisions
- C++17 standard for broad compiler compatibility
- Header-only design for easy integration into embedded systems
- Deterministic hashing for reproducible audit verification
- Single-writer thread model (typical for device firmware)

---

[1.0.0]: https://github.com/BigDataPlumbing/device_trust_shim/releases/tag/v1.0.0

