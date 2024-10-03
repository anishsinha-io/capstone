const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "roguelike",
        .target = target,
        .optimize = optimize,
    });

    const test_exe = b.addExecutable(.{
        .name = "roguelike_test",
        .target = target,
        .optimize = optimize,
    });

    exe.addCSourceFiles(.{
        .root = b.path("."),
        .files = &.{
            "main.cc",
            "src/game.cc",
            "src/player.cc",
            "src/gameobject.cc",
            "src/events.cc",
        },
    });
    exe.linkLibC();
    exe.linkLibCpp();

    test_exe.addCSourceFiles(.{
        .root = b.path("test"),
        .files = &.{
            "main_test.cc",
        },
    });
    test_exe.linkLibCpp();

    const googletest = b.dependency("googletest", .{
        .target = target,
        .optimize = optimize,
    });

    const gtest = b.addStaticLibrary(.{
        .name = "gtest",
        .target = target,
        .optimize = optimize,
    });

    gtest.linkLibCpp();
    gtest.addCSourceFiles(
        .{
            .root = googletest.path("googletest/src"),
            .files = &.{
                "gtest-all.cc",
                "gtest-assertion-result.cc",
                "gtest-death-test.cc",
                "gtest-filepath.cc",
                // LOL @ this bug
                // "gtest-internal-inl.h",
                "gtest-matchers.cc",
                "gtest-port.cc",
                "gtest-printers.cc",
                "gtest-test-part.cc",
                "gtest-typed-test.cc",
                "gtest.cc",
                "gtest_main.cc",
            },
            .flags = &.{
                "-std=c++17",
            },
        },
    );

    gtest.addIncludePath(googletest.path("googletest"));
    gtest.addIncludePath(googletest.path("googletest/src"));
    gtest.addIncludePath(googletest.path("googletest/include"));
    gtest.addIncludePath(googletest.path("googletest/include/internal"));
    gtest.addIncludePath(googletest.path("googletest/include/internal/custom"));

    const gmock = b.addStaticLibrary(.{
        .name = "gmock",
        .target = target,
        .optimize = optimize,
    });

    gmock.linkLibCpp();

    gmock.addCSourceFiles(
        .{
            .root = googletest.path("googlemock/src"),
            .files = &.{
                "gmock-all.cc",
                "gmock-cardinalities.cc",
                "gmock-internal-utils.cc",
                "gmock-matchers.cc",
                "gmock-spec-builders.cc",
                "gmock.cc",
                "gmock_main.cc",
            },
            .flags = &.{
                "-std=c++17",
            },
        },
    );

    gmock.addIncludePath(googletest.path("googlemock"));
    gmock.addIncludePath(googletest.path("googlemock/include"));
    gmock.addIncludePath(googletest.path("googlemock/include/gmock"));
    gmock.addIncludePath(googletest.path("googlemock/include/gmock/internal"));
    gmock.addIncludePath(googletest.path("googlemock/include/gmock/internal/custom"));
    gmock.addIncludePath(googletest.path("googletest"));
    gmock.addIncludePath(googletest.path("googletest/src"));
    gmock.addIncludePath(googletest.path("googletest/include"));
    gmock.addIncludePath(googletest.path("googletest/include/internal"));
    gmock.addIncludePath(googletest.path("googletest/include/internal/custom"));

    const raylib = b.dependency("raylib", .{
        .target = target,
        .optimize = optimize,
    });

    const raylibArtifact = raylib.artifact("raylib");

    const raylibCpp = b.dependency("raylib-cpp", .{
        .target = target,
        .optimize = optimize,
    });

    exe.linkLibrary(raylibArtifact);
    exe.addIncludePath(raylibCpp.path("include"));
    exe.addIncludePath(b.path("include"));

    test_exe.linkLibrary(gtest);
    test_exe.linkLibrary(gmock);
    test_exe.addIncludePath(googletest.path("googletest/include"));
    test_exe.addIncludePath(googletest.path("googlemock/include"));

    b.installArtifact(raylibArtifact);
    b.installDirectory(
        .{
            .source_dir = raylibCpp.path("include"),
            .install_dir = .header,
            .install_subdir = "",
        },
    );

    b.installDirectory(
        .{
            .source_dir = googletest.path("googletest/include"),
            .install_dir = .header,
            .install_subdir = "",
        },
    );

    b.installDirectory(
        .{
            .source_dir = googletest.path("googlemock/include"),
            .install_dir = .header,
            .install_subdir = "",
        },
    );

    b.installArtifact(exe);

    b.installArtifact(test_exe);

    const run = b.addRunArtifact(exe);
    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run.step);

    const run_test = b.addRunArtifact(test_exe);
    const run_test_step = b.step("test", "Run the tests");
    run_test_step.dependOn(&run_test.step);
}
