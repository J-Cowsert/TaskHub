#pragma once

#include <string>
#include <memory>

namespace taskhub {

	class FileDialog {
	public:
		virtual ~FileDialog() = default;

		virtual std::string OpenFile() = 0;

		static std::unique_ptr<FileDialog> Create();
	};
}