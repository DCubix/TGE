// This is a C++ wrapper of the PhysicsFS library
// The code is not mine, I took it from here: 
//		https://github.com/kahowell/physfs-cpp
// And made some changes, like adding a addToSearchPath method
// that was missing for some reason...

#ifndef PHYSFS_H
#define PHYSFS_H

#include <physfs.h>
#include <string>
#include <vector>
#include <iostream>

namespace PhysFS {

	enum mode {
		READ,
		WRITE,
		APPEND
	};

	typedef std::vector<std::string> StringList;

	typedef PHYSFS_uint8 uint8;

	typedef PHYSFS_sint8 sint8;

	typedef PHYSFS_uint16 uint16;

	typedef PHYSFS_sint16 sint16;

	typedef PHYSFS_uint32 uint32;

	typedef PHYSFS_sint32 sint32;

	typedef PHYSFS_uint64 uint64;

	typedef PHYSFS_sint64 sint64;

	typedef PHYSFS_StringCallback StringCallback;

	typedef PHYSFS_EnumFilesCallback EnumFilesCallback;

	typedef PHYSFS_Version Version;

	typedef PHYSFS_Allocator Allocator;

	typedef PHYSFS_ArchiveInfo ArchiveInfo;

	typedef std::vector<ArchiveInfo> ArchiveInfoList;

	typedef uint64 size_t;

	class base_fstream {
	protected:
		PHYSFS_File * const file;
	public:
		base_fstream(PHYSFS_File * file);
		virtual ~base_fstream();
		std::size_t length();
	};

	class ifstream : public base_fstream, public std::istream {
	public:
		ifstream(std::string const & filename);
		virtual ~ifstream();
	};

	class ofstream : public base_fstream, public std::ostream {
	public:
		ofstream(std::string const & filename, mode writeMode = WRITE);
		virtual ~ofstream();
	};

	class fstream : public base_fstream, public std::iostream {
	public:
		fstream(std::string const & filename, mode openMode = READ);
		virtual ~fstream();
	};

	Version getLinkedVersion();

	void init(char const * argv0);

	void deinit();

	ArchiveInfoList supportedArchiveTypes();

	std::string getDirSeparator();

	void permitSymbolicLinks(bool allow);

	StringList getCdRomDirs();

	void getCdRomDirs(StringCallback callback, void * extra);

	std::string getBaseDir();

	std::string getUserDir();

	std::string getWriteDir();

	void setWriteDir(std::string const & newDir);

	void addToSearchPath(std::string const & newDir, bool append = true);

	void removeFromSearchPath(std::string const & oldDir);

	StringList getSearchPath();

	void getSearchPath(StringCallback callback, void * extra);

	void setSaneConfig(std::string const & orgName, std::string const & appName, std::string const & archiveExt, bool includeCdRoms, bool archivesFirst);

	void mkdir(std::string const & dirName);

	void deleteFile(std::string const & filename);

	std::string getRealDir(std::string const & filename);

	StringList enumerateFiles(std::string const & directory);

	void enumerateFiles(std::string const & directory, EnumFilesCallback callback, void * extra);

	bool exists(std::string const & filename);

	bool isDirectory(std::string const & filename);

	bool isSymbolicLink(std::string const & filename);

	sint64 getLastModTime(std::string const & filename);

	bool isInit();

	bool symbolicLinksPermitted();

	void setAllocator(Allocator const * allocator);

	void mount(std::string const & newDir, std::string const & mountPoint, bool appendToPath);

	std::string getMountPoint(std::string const & dir);

	namespace Util {

		sint16 swapSLE16(sint16 value);

		uint16 swapULE16(uint16 value);

		sint32 swapSLE32(sint32 value);

		uint32 swapULE32(uint32 value);

		sint64 swapSLE64(sint64 value);

		uint64 swapULE64(uint64 value);

		sint16 swapSBE16(sint16 value);

		uint16 swapUBE16(uint16 value);

		sint32 swapSBE32(sint32 value);

		uint32 swapUBE32(uint32 value);

		sint64 swapSBE64(sint64 value);

		uint64 swapUBE64(uint64 value);

		std::string utf8FromUcs4(uint32 const * src);

		std::string utf8ToUcs4(char const * src);

		std::string utf8FromUcs2(uint16 const * src);

		std::string utf8ToUcs2(char const * src);

		std::string utf8FromLatin1(char const * src);

	}

}

#endif