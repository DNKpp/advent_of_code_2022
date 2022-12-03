set(TARGET_NAME ${PROJECT_NAME}_02)

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

CPMGetPackage(fmt)
CPMGetPackage(range-v3)
target_link_libraries(
	${TARGET_NAME}
	PRIVATE
	fmt::fmt
	range-v3::range-v3
)
