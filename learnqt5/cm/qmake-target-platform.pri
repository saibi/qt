win32 {
	CONFIG += PLATFORM_WIN
	message(PLATFORM_WIN)
	win32-g++ {
		CONFIG += COMPILER_GCC
		message(COMPILER_GCC)
	}
	win32-msvc2017 {
		CONFIG += COMPILER_MSVC2017
		message(COMPILER_MSVC2017)
		win32-msvc2017:QMAKE_TARGET.arch = x86_64
	}
}

linux {
	CONFIG += PLATFORM_LINUX
	message(PLATFORM_LINUX)

	!contains(QT_ARCH, x86_64) {
		QMAKE_TARGET.arch = x86 
	} else {
		QMAKE_TARGET.arch = x86_64 
	}
	linux-g++ {
		CONFIG += COMPILER_GCC
		message(COMPILER_GCC)
	}
}

contains(QMAKE_TARGET.arch, x86_64) {
	CONFIG += PROCESSOR_x64
	message(PROCESSOR_x64)
} else {
	CONFIG += PROCESSOR_x86
	message(PROCESSOR_x86)
}

CONFIG(debug, release|debug) {
	CONFIG += BUILD_DEBUG
	message(BUILD_DEBUG)
} else {
	CONFIG += BUILD_RELEASE
	message(BUILD_RELEASE)
}


