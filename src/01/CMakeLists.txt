add_executable(
    ${PROJECT_NAME}_01
    "main.cpp"
)

target_compile_definitions(
	${PROJECT_NAME}_01
	PRIVATE
	INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_features(
	${PROJECT_NAME}_01
	PUBLIC
	cxx_std_20
)

CPMGetPackage(fmt)
CPMGetPackage(range-v3)
target_link_libraries(
	${PROJECT_NAME}_01
	PRIVATE
	fmt::fmt
	range-v3::range-v3
)
