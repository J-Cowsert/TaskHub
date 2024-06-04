#pragma once

#include <memory>

namespace taskhub {

	struct Buffer {

		void* Data;
		uint64_t Size;

		Buffer()
			: Data(nullptr), Size(0)
		{
		}

		Buffer(const void* data, uint64_t size)
			: Data((void*)data), Size(size)
		{
		}
		
		static Buffer Copy(const void* data, uint64_t size) {

			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, data, size);
			return buffer;
		}

		void Allocate(uint64_t size) {

			delete[](uint8_t*)Data;
			Data = nullptr;
			if (size == 0)
				return;
			Data = new uint8_t[size];
			Size = size;
		}

		void Deallocate() {

			delete[](uint8_t*)Data;
			Data = nullptr;
			Size = 0;
		}
	};
}