# This script drives creation of a git repository and checks
# that CTest can update from it.

#-----------------------------------------------------------------------------
# Test in a directory next to this script.
get_filename_component(TOP "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(TOP "${TOP}/CTest UpdateGIT")
set(UPDATE_EXTRA Updated{module})

# Include code common to all update tests.
include("/Users/davidcoplowe/software/xsLLhFitter/cmake/CMAKE/Darwin/cmake-3.5.0/Tests/CTestUpdateCommon.cmake")

#-----------------------------------------------------------------------------
# Report git tools in use.
message("Using GIT tools:")
set(GIT "/usr/local/git/bin/git")
message(" git = ${GIT}")

set(AUTHOR_CONFIG "[user]
\tname = Test Author
\temail = testauthor@cmake.org
")

#-----------------------------------------------------------------------------
# Initialize the testing directory.
message("Creating test directory...")
init_testing()

if(UNIX)
  set(src "/Users/davidcoplowe/software/xsLLhFitter/cmake/CMAKE/Darwin/cmake-3.5.0/Tests")
  configure_file(${src}/CTestUpdateGIT.sh.in ${TOP}/git.sh @ONLY)
  set(GIT ${TOP}/git.sh)
endif()

#-----------------------------------------------------------------------------
# Create the repository.
message("Creating repository...")
file(MAKE_DIRECTORY ${TOP}/repo.git)
run_child(
  WORKING_DIRECTORY ${TOP}/repo.git
  COMMAND ${GIT} --bare init
  )
file(REMOVE_RECURSE ${TOP}/repo.git/hooks)

# Create submodule repository.
message("Creating submodule...")
file(MAKE_DIRECTORY ${TOP}/module.git)
run_child(
  WORKING_DIRECTORY ${TOP}/module.git
  COMMAND ${GIT} --bare init
  )
file(REMOVE_RECURSE ${TOP}/module.git/hooks)
run_child(WORKING_DIRECTORY ${TOP}
  COMMAND ${GIT} clone module.git module
  )
file(REMOVE_RECURSE ${TOP}/module/.git/hooks)
file(APPEND ${TOP}/module/.git/config "
${AUTHOR_CONFIG}")
create_content(module)
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} add .
  )
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} commit -m "Initial content"
  )
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} push origin master:refs/heads/master
  )

#-----------------------------------------------------------------------------
# Import initial content into the repository.
message("Importing content...")

# Import the content into the repository.
run_child(WORKING_DIRECTORY ${TOP}
  COMMAND ${GIT} clone repo.git import
  )
file(REMOVE_RECURSE ${TOP}/import/.git/hooks)
file(APPEND ${TOP}/import/.git/config "
${AUTHOR_CONFIG}")
create_content(import)
file(WRITE ${TOP}/import/HEAD "HEAD\n")
file(WRITE ${TOP}/import/master "master\n")
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} add .
  )
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} config core.safecrlf false
  )
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} submodule add ../module.git module
  )
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} commit -m "Initial content"
  )
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} push origin master:refs/heads/master
  )

#-----------------------------------------------------------------------------
# Modify the submodule.
change_content(module)
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} add -u
  )
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} commit -m "Changed content"
  )
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} push origin master:refs/heads/master
  )

#-----------------------------------------------------------------------------
# Create a working tree.
message("Checking out revision 1...")
run_child(
  WORKING_DIRECTORY ${TOP}
  COMMAND ${GIT} clone repo.git user-source
  )
file(REMOVE_RECURSE ${TOP}/user-source/.git/hooks)
file(APPEND ${TOP}/user-source/.git/config "${AUTHOR_CONFIG}")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} submodule init
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} submodule update
  )

# Save the first revision name.
execute_process(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} rev-parse HEAD
  OUTPUT_VARIABLE revision1
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

#-----------------------------------------------------------------------------
# Create an empty commit.
message("Creating empty commit...")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} commit --allow-empty -m "Empty commit"
  )

#-----------------------------------------------------------------------------
# Make changes in the working tree.
message("Changing content...")
update_content(user-source files_added files_removed dirs_added)
if(dirs_added)
  run_child(
    WORKING_DIRECTORY ${TOP}/user-source
    COMMAND ${GIT} add -- ${dirs_added}
    )
endif()
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} add -- ${files_added}
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} rm -- ${files_removed}
  )
run_child(WORKING_DIRECTORY ${TOP}/user-source/module
  COMMAND ${GIT} checkout master --
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} add -u
  )

#-----------------------------------------------------------------------------
# Commit the changes to the repository.
message("Committing revision 2...")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} commit -m "Changed content"
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} push origin
  )

#-----------------------------------------------------------------------------
# Make changes in the working tree.
message("Changing content again...")
change_content(user-source)
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} add -u
  )

#-----------------------------------------------------------------------------
# Commit the changes to the repository.
message("Committing revision 3...")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} commit -m "Changed content again"
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} push origin
  )

#-----------------------------------------------------------------------------
# Go back to before the changes so we can test updating.
macro(rewind_source src_dir)
  message("Backing up to revision 1...")
  run_child(
    WORKING_DIRECTORY ${TOP}/${src_dir}
    COMMAND ${GIT} reset --hard ${revision1}
    )
  run_child(
    WORKING_DIRECTORY ${TOP}/${src_dir}
    COMMAND ${GIT} submodule update
    )
endmacro()
rewind_source(user-source)

# Make sure pull does not try to rebase (which does not work with
# modified files) even if ~/.gitconfig sets "branch.master.rebase".
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} config branch.master.rebase false
  )

# Create a modified file.
modify_content(user-source)

#-----------------------------------------------------------------------------
# Test updating the user work directory with the command-line interface.
message("Running CTest Dashboard Command Line...")

# Create the user build tree.
create_build_tree(user-source user-binary)
file(APPEND ${TOP}/user-binary/CTestConfiguration.ini
  "# GIT command configuration
UpdateCommand: ${GIT}
")

# Run the dashboard command line interface.
set(UPDATE_NO_MODIFIED 1)
run_dashboard_command_line(user-binary)
set(UPDATE_NO_MODIFIED 0)

rewind_source(user-source)
modify_content(user-source)

message("Running CTest Dashboard Command Line (custom update)...")

# Create the user build tree.
create_build_tree(user-source user-binary-custom)
file(APPEND ${TOP}/user-binary-custom/CTestConfiguration.ini
  "# GIT command configuration
UpdateCommand: ${GIT}
GITUpdateCustom: ${GIT};pull;origin;master
")

# Run the dashboard command line interface.
run_dashboard_command_line(user-binary-custom)

#-----------------------------------------------------------------------------
# Test initial checkout and update with a dashboard script.
message("Running CTest Dashboard Script...")

create_dashboard_script(dash-binary
  "# git command configuration
set(CTEST_GIT_COMMAND \"${GIT}\")
set(CTEST_GIT_UPDATE_OPTIONS)
execute_process(
  WORKING_DIRECTORY \"${TOP}\"
  COMMAND \"${GIT}\" clone repo.git dash-source
  )

# Test .git file.
file(RENAME \"${TOP}/dash-source/.git\" \"${TOP}/dash-source/repo.git\")
file(WRITE \"${TOP}/dash-source/.git\" \"gitdir: repo.git\n\")

execute_process(
  WORKING_DIRECTORY \"${TOP}/dash-source\"
  COMMAND \"${GIT}\" reset --hard ${revision1}
  )
execute_process(
  WORKING_DIRECTORY \"${TOP}/dash-source\"
  COMMAND \"${GIT}\" submodule init
  )
execute_process(
  WORKING_DIRECTORY \"${TOP}/dash-source\"
  COMMAND \"${GIT}\" submodule update
  )
")

# Run the dashboard script with CTest.
run_dashboard_script(dash-binary)

rewind_source(dash-source)

#-----------------------------------------------------------------------------
# Test custom update with a dashboard script.
message("Running CTest Dashboard Script (custom update)...")

create_dashboard_script(dash-binary-custom
  "# git command configuration
set(CTEST_GIT_COMMAND \"${GIT}\")
set(CTEST_GIT_UPDATE_OPTIONS)
set(CTEST_GIT_UPDATE_CUSTOM \${CTEST_GIT_COMMAND} pull origin master)
")

# Run the dashboard script with CTest.
run_dashboard_script(dash-binary-custom)


rewind_source(dash-source)

#-----------------------------------------------------------------------------
# Test no update with a dashboard script.
message("Running CTest Dashboard Script (No update)...")

create_dashboard_script(dash-binary-no-update
  "# git command configuration
set(CTEST_GIT_COMMAND \"${GIT}\")
set(CTEST_UPDATE_VERSION_ONLY TRUE)
")

# Run the dashboard script with CTest.
set(NO_UPDATE 1)
run_dashboard_script(dash-binary-no-update)
unset(NO_UPDATE)

rewind_source(dash-source)

#-----------------------------------------------------------------------------
# Test ctest_update(QUIET)
message("Running CTest Dashboard Script (update quietly)...")

set(ctest_update_args QUIET)
create_dashboard_script(dash-binary-quiet
  "# git command configuration
set(CTEST_GIT_COMMAND \"${GIT}\")
set(CTEST_GIT_UPDATE_OPTIONS)
set(CTEST_GIT_UPDATE_CUSTOM \${CTEST_GIT_COMMAND} pull origin master)
")
unset(ctest_update_args)

# Run the dashboard script with CTest.
run_dashboard_script(dash-binary-quiet)

# Make sure the output seems quiet.
if("${OUTPUT}" MATCHES "Updating the repository")
  message(FATAL_ERROR "Found 'Updating the repository' in quiet output")
endif()

#-----------------------------------------------------------------------------
# Test ctest_update(RETURN_VALUE) on failure
message("Running CTest Dashboard Script (fail to update)...")

set(ctest_update_check [[

if(NOT ret LESS 0)
  message(FATAL_ERROR "ctest_update incorrectly succeeded with ${ret}")
endif()
]])
create_dashboard_script(dash-binary-fail
  "set(CTEST_GIT_COMMAND \"update-command-does-not-exist\")
")
unset(ctest_update_check)

# Run the dashboard script with CTest.
set(FAIL_UPDATE 1)
run_dashboard_script(dash-binary-fail)
unset(FAIL_UPDATE)
