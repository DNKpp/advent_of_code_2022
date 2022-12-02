CPMDeclarePackage(
	Catch2
	GIT_TAG v3.2.0
	GITHUB_REPOSITORY catchorg/Catch2
	EXCLUDE_FROM_ALL YES
)

CPMDeclarePackage(
	CTRE
	NAME CTRE
	GIT_TAG v3.7.1
	GITHUB_REPOSITORY "hanickadot/compile-time-regular-expressions"
	EXCLUDE_FROM_ALL YES
)

CPMDeclarePackage(
	range-v3
	GIT_TAG 0.12.0
	GITHUB_REPOSITORY ericniebler/range-v3
	EXCLUDE_FROM_ALL YES
	OPTIONS
		"RANGES_BUILD_CALENDAR_EXAMPLE OFF"
)

CPMDeclarePackage(
	Simple-Utility
	GIT_TAG v2.0.3
	GITHUB_REPOSITORY DNkpp/Simple-Utility
	EXCLUDE_FROM_ALL YES
)

CPMDeclarePackage(
	fmt
	GIT_TAG 9.1.0
	GITHUB_REPOSITORY fmtlib/fmt
	EXCLUDE_FROM_ALL YES
)
