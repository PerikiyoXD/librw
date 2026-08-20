-- Third-party sources vendored into the tree.
--
-- Each is a self-contained static target exporting its own public includedir;
-- nothing outside this directory puts "vendor" on an include path. All are
-- set_default(false), so they build only when depended on.
--
-- Why vendored rather than xrepo packages:
--   glad     generator output pinned to gl=3.3/gles2=3.1 core
--   lodepng  single file, and the build depends on LODEPNG_NO_COMPILE_CPP
--   imgui    tracked by hand against librw's own renderer backend

includes("glad", "lodepng", "imgui")
