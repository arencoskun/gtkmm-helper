#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
namespace filesystem = boost::filesystem;

const std::string cmakeListsTxtFile = R"(
# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.1.0)

# Set the minimum required C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define project name
project(REPLACE_ME_WITH_PROJECT_NAME)

# Find the gtkmm package
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

# Add source files
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Include gtkmm directories
include_directories(${GTKMM_INCLUDE_DIRS})

# Include headers in the include/ directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create an executable from main.cpp
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

# Link against gtkmm libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES})
)";

const std::string mainCppFile = R"(
#include <gtkmm.h>

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  Gtk::Window window;

  Gtk::Main::run(window);

  return EXIT_SUCCESS;
}
)";

/**
 * Return the content of a CMakeLists.txt file with the desired project name.
 *
 * @param name The name of the project.
 * @return A string containing the content of the CMakeLists.txt file.
 */
std::string getCmakeListsTxt(std::string name) {
	std::string cmakeListsTxtFileCopy = cmakeListsTxtFile;
	std::string token("REPLACE_ME_WITH_PROJECT_NAME");
	size_t pos = cmakeListsTxtFileCopy.find(token);

    if (pos != std::string::npos) {
        cmakeListsTxtFileCopy.replace(pos, token.size(), name);
    }

    return cmakeListsTxtFileCopy;
}

/**
 * Create a file at the desired path and write the content passed through.
 *
 * @param filePath The path of the file without the filename.
 * @param filename The desired filename.
 * @param content The content that is to be written to the newly created file.
 */
void createFileAndWriteContent(filesystem::path filePath, std::string filename, std::string content) {
	filesystem::ofstream file(filePath / filename);
    if (file.is_open()) {
        file << content;
        file.close();
    } else {
        std::cerr << "error: unable to open file " << filename << "\n";
    }
}

/**
 * Create a simple gtkmm project with a CMakeLists.txt file.
 *
 * @param name The name of the project.
 * @param targetPath The path for the new project.
 */
void createProject(std::string name, filesystem::path targetPath) {
    filesystem::path projectPath = targetPath / name;
    filesystem::path projectSrcPath = projectPath / "src";
    filesystem::path projectIncludePath = projectPath / "include";
    filesystem::path projectBuildPath = projectPath / "build";

    filesystem::create_directory(projectPath);
    filesystem::create_directory(projectSrcPath);
    filesystem::create_directory(projectIncludePath);
    filesystem::create_directory(projectBuildPath);

    createFileAndWriteContent(projectPath, "CMakeLists.txt", getCmakeListsTxt(name));
    createFileAndWriteContent(projectSrcPath, "main.cpp", mainCppFile);

    std::cout << "the project was created successfully, thanks for using gtkmm-helper!" << "\n" << "\n";
    std::cout << "you can build the project using the commands below." << "\n";
    std::cout << "$ cd " << filesystem::path(projectPath / "build") << "\n";
    std::cout << "$ cmake .." << "\n";
    std::cout << "$ make" << "\n";
    std::cout << "you can then run the project." << "\n";
    std::cout << "$ ./" << name << "\n";
}

int main(int argc, char* argv[]) {
    try {
        po::options_description desc("allowed options");
        desc.add_options()
        ("help,h", "produce help message")
        ("new,n", po::value<std::string>(), "create new project")
        ("dir,d", po::value<std::string>(), "set target directory for new project. if not set, the project will be created in the current directory.")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (argc == 1) {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("new")) {
            std::string projectName = vm["new"].as<std::string>();
            if (vm.count("dir")) {
                std::string projectDir = vm["dir"].as<std::string>();
                createProject(projectName, filesystem::path(projectDir));
            } else {
                createProject(projectName, filesystem::current_path());
            }
        } else if (vm.count("dir")) {
            std::cerr << "error: --dir option can only be used after --new option.\n";
            return 1;
        }
    } catch(const po::error& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    } catch(...) {
        std::cerr << "unknown error - please try again.\n" << "\n";
        return 1;
    }

    return 0;
}
