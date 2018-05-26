Changelog For `zero/logger.h`
=============================

Version numbers comply with the [Sementic Versioning Specification (SemVer)].


## [v0.2.0] (2018-05-26)

### Added

* Log level 'trace'.
* Log messages.


### Changed

* Swap the color for the 'info' log level with 'trace'.
* Refer directly to structs and enums instead of using typedefs.
* Reverse the log level ordering.
* Rename the macro `ZR_DISABLE_LOG_COLORING` to `ZR_DISABLE_LOG_STYLING`.
* Rename the macro `ZR_IMPLEMENTATION` to `ZR_DEFINE_IMPLEMENTATION`.
* Rename the macros `ZR_LOGGER_STATIC` and `ZR_STATIC` respectively to
  `ZR_LOGGER_SPECIFY_INTERNAL_LINKAGE` and `ZR_SPECIFY_INTERNAL_LINKAGE`.
* Don't define the `extern "C"` as a namespace.
* Apply minor internal changes.


### Removed

* Module-specific macro `ZR_LOGGER_ASSERT`.


## [v0.1.1] (2018-03-13)

## Added

* Make macros definable for this module only, overriding the project-level ones.


### Changed

* Check for the POSIX.1 feature set.
* Guard against warnings from `-Wunused-function`.
* Guard against warnings from `-Wundef`.
* Wrap all private code into the module's namespace.
* Apply minor internal changes.


## v0.1.0 (2018-01-20)

* Initial release.


[Sementic Versioning Specification (SemVer)]: https://semver.org
[v0.2.0]: https://github.com/christophercrouzet/zero/compare/logger-v0.1.1...logger-v0.2.0
[v0.1.1]: https://github.com/christophercrouzet/zero/compare/logger-v0.1.0...logger-v0.1.1
