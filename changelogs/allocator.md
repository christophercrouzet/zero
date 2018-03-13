Changelog For `zero/allocator.h`
================================

Version numbers comply with the [Sementic Versioning Specification (SemVer)].


## [v0.1.1] (2018-03-13)

## Added

* Make macros definable for this module only, overriding the project-level ones.


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
[v0.1.1]: https://github.com/christophercrouzet/zero/compare/allocator-v0.1.0...allocator-v0.1.1
