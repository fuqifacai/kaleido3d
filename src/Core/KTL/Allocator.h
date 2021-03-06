/**
 * MIT License
 *
 * Copyright (c) 2019 Zhou Qin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once
#if __cplusplus
extern "C" {
#endif
extern K3D_CORE_API void*   k3d_malloc(size_t SzOfObj);
extern K3D_CORE_API void*   k3d_malloc_aligned(size_t SzOfObj, int Alignment);
extern K3D_CORE_API void    k3d_free(void *Ptr, size_t SzOfObj);
#if __cplusplus
}
#endif
extern K3D_CORE_API void* operator new(size_t Size, const char* _ClassName, const char* _SourceFile, int _SourceLine);
extern K3D_CORE_API void* operator new[](size_t Size, const char* pName);
extern K3D_CORE_API void operator delete(void* _Ptr, const char* _SourceFile, int _SourceLine);

#define K3D_NEW(_Class) new (#_Class, __FILE__, __LINE__) _Class
#define K3D_DELETE(_Ptr) delete (__FILE__, __LINE__) _Ptr

namespace k3d
{
	template<typename U>
	struct DefaultDeletor
	{
		DefaultDeletor() = default;
		void operator()(U * pObj)
		{
			delete pObj;
		}
	};

	template<typename U>
	struct DefaultDeletor<U[]>
	{
		DefaultDeletor() = default;
		void operator()(U * pObj)
		{
			delete[] pObj;
		}
	};

    struct IAllocatorAdapter
    {
        virtual ~IAllocatorAdapter() {}

        virtual void*       Alloc(size_t SzToAlloc, int Alignment = 0, int AlignOffset = 0, int Flags = 0, const char* AllocInfo = nullptr) = 0;
        virtual void        DeAlloc(void* Ptr) = 0;
        virtual const char* GetName() const = 0;
    };

    extern K3D_CORE_API IAllocatorAdapter& GetDefaultAllocator();

	class kAllocator
	{
	public:
		kAllocator(const char* = nullptr) {}
		kAllocator(const kAllocator&) {}
		kAllocator(const kAllocator&, const char*) {}
		kAllocator& operator=(const kAllocator&) { return *this; }
		bool operator==(const kAllocator&) { return true; }
		bool operator!=(const kAllocator&) { return false; }
		
        void* allocate(size_t n, int /*flags = 0*/)
        { 
            return GetDefaultAllocator().Alloc(n); 
        }
		
        void* allocate(size_t n, size_t /*alignment*/, size_t /*alignmentOffset*/, int /*flags = 0*/)
		{
			return GetDefaultAllocator().Alloc(n);
		}
		
        void deallocate(void* p, size_t n) { GetDefaultAllocator().DeAlloc(p); }
		
        const char* get_name() const { return "kAllocator"; }
		void set_name(const char*) {}
    };

    template<size_t N, size_t Align = alignof(double)>
    struct AlignedStorage
    {
        typedef struct {
            alignas(Align) unsigned char CharData[N];
        } Type;
    };
}
