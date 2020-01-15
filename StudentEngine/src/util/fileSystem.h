#pragma once

class FileSystem {
public:
	struct FileDialogData {
		bool m_success;
		String m_file;
	};

	static String ReadFile(String path) {
		ifstream stream(path);
		string str((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
		stream.close();
		return str;
	}

	static bool DoesFileExist(const String& path) {
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);
	}

	static void SaveJsonToFile(const nlohmann::json& jsonOb, const String& name);

	static nlohmann::json LoadJsonFromFile(const String& name);

	static void SaveStringToFile(const String& str, const String& name);
	static void CreateFile(const String& file);

	static bool CreateFileIfDoesntExist(const String& file) {
		bool exists = DoesFileExist(file);
		if (!exists) CreateFile(file);
		return !exists;
	}

	static String_t GetShortFilename(const char* filename) {
		const char* lastSlash = strrchr(filename, '/');
		if (lastSlash == nullptr) {
			lastSlash = strrchr(filename, '\\');
		}
		String_t shortFilename = lastSlash != nullptr ? lastSlash + 1 : filename;
		return shortFilename;
	}

	static FileDialogData SaveFileDialog() {
		FileDialogData toRet{ false, "" };
		HRESULT hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hResult)) {
			IFileSaveDialog* fileSaveDialog;

			hResult = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&fileSaveDialog));

			COMDLG_FILTERSPEC rgSpec[] = {
				{ L"Project files", L"*.json" }
			};

			fileSaveDialog->SetFileTypes(1, rgSpec);

			if (SUCCEEDED(hResult)) {
				hResult = fileSaveDialog->Show(NULL);
				if (SUCCEEDED(hResult)) {
					IShellItem* shellItem;
					hResult = fileSaveDialog->GetResult(&shellItem);
					if (SUCCEEDED(hResult)) {
						PWSTR filePath;
						hResult = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

						if (SUCCEEDED(hResult)) {
							toRet.m_success = true;
							wstring wstr = wstring(filePath);
							toRet.m_file = string(wstr.begin(), wstr.end());
							CoTaskMemFree(filePath);
						}
						shellItem->Release();
					}
				}
				fileSaveDialog->Release();
			}
			CoUninitialize();
		}
		return toRet;
	}
	
	static FileDialogData OpenFileDialog() {
		FileDialogData toRet{ false, "" };
		HRESULT hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hResult)) {
			IFileOpenDialog* fileOpenDialog;

			hResult = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				IID_IFileOpenDialog, reinterpret_cast<void**>(&fileOpenDialog));

			COMDLG_FILTERSPEC rgSpec[] = {
				{ L"Project files", L"*.json" }
			};
			
			fileOpenDialog->SetFileTypes(1, rgSpec);

			if (SUCCEEDED(hResult)) {
				hResult = fileOpenDialog->Show(NULL);

				if (SUCCEEDED(hResult)) {
					IShellItem* shellItem;
					hResult = fileOpenDialog->GetResult(&shellItem);
					if (SUCCEEDED(hResult)) {
						PWSTR filePath;
						hResult = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

						if (SUCCEEDED(hResult)) {
							toRet.m_success = true;
							wstring wstr = wstring(filePath);
							toRet.m_file = string(wstr.begin(), wstr.end());
							CoTaskMemFree(filePath);
						}
						shellItem->Release();
					}
				}
				fileOpenDialog->Release();
			}
			CoUninitialize();
		}
		return toRet;
	}
};
