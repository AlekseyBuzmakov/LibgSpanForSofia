#!lua

-- A solution contains projects, and defines the available configurations
solution "LibgSpanForSofia"
	location "build"
	language "C++"

	configurations { "release", "debug" }

	function DefaultConfig(complimentName)
		configuration "Debug"
			defines { "DEBUG", "_DEBUG" }
			flags { "Symbols" }
			targetdir ("build/debug/" .. complimentName)

		configuration "Release"
			defines { "NDEBUG", "BOOST_DISABLE_ASSERTS" }
			flags { "Optimize" } -- ,"Symbols" }
			targetdir ("build/release/" .. complimentName)


		configuration "*"
	end

	project "LibgSpanForSofia"
		DefaultConfig("lib")
		kind "SharedLib"
		includedirs { 
			"inc/",
			"src/"
		}
		files{ "src/**.cpp" }

		configuration "Debug"
			targetname( "LibgSpanForSofia" )

		configuration "Release"
			targetname( "LibgSpanForSofia" )

	project "test"
		DefaultConfig("bin")
		kind "ConsoleApp"
		includedirs { 
			"inc/",
			"src/"
		}
		files{ "src/**.cpp" }

		configuration "Debug"
			targetname( "test" )

		configuration "Release"
			targetname( "test" )
