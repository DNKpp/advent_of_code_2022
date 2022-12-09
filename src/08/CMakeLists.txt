set(TARGET_NAME ${PROJECT_NAME}_08)

add_executable(
    ${TARGET_NAME}
    "main.cpp"
)

target_include_directories(
	${TARGET_NAME}
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_compile_definitions(
	${TARGET_NAME}
	PRIVATE
	INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_features(
	${TARGET_NAME}
	PUBLIC
	cxx_std_20
)

CPMGetPackage(CTRE)
CPMGetPackage(fmt)
CPMGetPackage(mdspan)
CPMGetPackage(range-v3)
CPMGetPackage(Simple-Utility)
target_link_libraries(
	${TARGET_NAME}
	PRIVATE
	fmt::fmt
	range-v3::range-v3
	Simple-Utility::Simple-Utility
	ctre::ctre
	std::mdspan
)
