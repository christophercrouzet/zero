Changelog For `zero/timer.h`
============================

Version numbers comply with the [Sementic Versioning Specification (SemVer)].


## [v0.2.0] (2018-05-26)

### Added

* Log messages.


### Changed

* Guard against `_POSIX_C_SOURCE` being not defined.
* Refer directly to structs and enums instead of using typedefs.
* Rename `ZrResult` to `ZrStatus`.
* Rename the macro `ZR_IMPLEMENTATION` to `ZR_DEFINE_IMPLEMENTATION`.
* Rename the macros `ZR_TIMER_STATIC` and `ZR_STATIC` respectively to
  `ZR_TIMER_SPECIFY_INTERNAL_LINKAGE` and `ZR_SPECIFY_INTERNAL_LINKAGE`.
* Don't define the `extern "C"` as a namespace.


### Removed

* Module-specific macro `ZR_TIMER_ASSERT`.


## v0.1.0 (2018-03-13)

* Initial release.


[v0.2.0]: https://github.com/christophercrouzet/zero/compare/timer-v0.1.0...timer-v0.2.0
[Sementic Versioning Specification (SemVer)]: https://semver.org
