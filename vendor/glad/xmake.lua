-- glad -- generated GL/GLES loader, not hand-written.
--
-- Regenerate with the exact invocation glad recorded in the header preamble
-- of include/glad/glad.h -- read it there, do not retype it from here. It
-- carries five extensions and --no-loader; dropping any of them silently
-- removes symbols librw's consumers rely on:
--     --profile="core" --api="gl=3.3,gles2=3.1" --generator="c" --spec="gl"
--     --no-loader
--     --extensions="GL_EXT_framebuffer_object,GL_EXT_texture_compression_s3tc,
--                   GL_EXT_texture_filter_anisotropic,GL_KHR_debug,
--                   GL_KHR_texture_compression_astc_ldr"
-- Current export: glad 0.1.34, 2021-02-17.
--
-- Canonical glad layout, so the sources stay byte-identical to the generator
-- output: glad.h includes "khrplatform.h" from its own directory, and
-- src/glad.c includes "glad.h" from its own -- hence the private includedir.

target("glad")
    set_kind("static")
    set_group("vendor")
    set_default(false)
    set_languages("c11")

    add_includedirs("include", {public = true})
    add_includedirs("include/glad")

    add_files("src/glad.c")

    add_headerfiles("include/(glad/*.h)")
target_end()
