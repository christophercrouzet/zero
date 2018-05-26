Changelog For `zero/allocator.h`
================================

Version numbers comply with the [Sementic Versioning Specification (SemVer)].


## [v0.2.0] (2018-05-26)

### Added

* Global macro `ZR_ENABLE_DEBUGGING`.
* Log messages.


### Changed

* Refer directly to structs and enums instead of using typedefs.
* `zrFree()` and `zrFreeAligned()` now accept a const pointer to `void`.
* Rename the macro `ZR_IMPLEMENTATION` to `ZR_DEFINE_IMPLEMENTATION`.
* Rename the macros `ZR_ALLOCATOR_STATIC` and `ZR_STATIC` respectively to
  `ZR_ALLOCATOR_SPECIFY_INTERNAL_LINKAGE` and `ZR_SPECIFY_INTERNAL_LINKAGE`.
* Don't define the `extern "C"` as a namespace.
* Apply minor internal changes.


### Removed

* Module-specific macros such as `ZR_ALLOCATOR_ASSERT`, `ZR_ALLOCATOR_MALLOC`,
  `ZR_ALLOCATOR_REALLOC`, and `ZR_ALLOCATOR_FREE`.


## [v0.1.1] (2018-03-13)

## Added

* Module-specific macros, overriding the global ones.


### Changed

* Rewrite the power of two check.
* Guard against warnings from `-Wunused-function`.
* Make the architecture macros private.
* Make static assertion messages private types.
* Wrap all private code into the module's namespace.
* Apply minor internal changes.


## v0.1.0 (2018-01-20)

* Initial release.


[Sementic Versioning Specification (SemVer)]: https://semver.org
[v0.2.0]: https://github.com/christophercrouzet/zero/compare/allocator-v0.1.1...allocator-v0.2.0
[v0.1.1]: https://github.com/christophercrouzet/zero/compare/allocator-v0.1.0...allocator-v0.1.1
