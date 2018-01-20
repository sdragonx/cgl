//===-- ConvertUTFWrapper.cpp - Wrap ConvertUTF.h with clang data types -----===
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//#include "llvm/Support/ConvertUTF.h"
//#include "llvm/Support/SwapByteOrder.h"
#include <string>
#include <vector>


/*************************************************************************/
/* Below are LLVM-specific wrappers of the functions above. */

#ifdef CONVERT_UTF_WRAPPER

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"

//namespace llvm {

/**
 * Convert an UTF8 StringRef to UTF8, UTF16, or UTF32 depending on
 * WideCharWidth. The converted data is written to ResultPtr, which needs to
 * point to at least WideCharWidth * (Source.Size() + 1) bytes. On success,
 * ResultPtr will point one after the end of the copied string. On failure,
 * ResultPtr will not be changed, and ErrorPtr will be set to the location of
 * the first character which could not be converted.
 * \return true on success.
 */
bool ConvertUTF8toWide(unsigned WideCharWidth, llvm::StringRef Source,
                       char *&ResultPtr, const UTF8 *&ErrorPtr);

/**
 * Convert an Unicode code point to UTF8 sequence.
 *
 * \param Source a Unicode code point.
 * \param [in,out] ResultPtr pointer to the output buffer, needs to be at least
 * \c UNI_MAX_UTF8_BYTES_PER_CODE_POINT bytes.  On success \c ResultPtr is
 * updated one past end of the converted sequence.
 *
 * \returns true on success.
 */
bool ConvertCodePointToUTF8(unsigned Source, char *&ResultPtr);

/**
 * Convert the first UTF8 sequence in the given source buffer to a UTF32
 * code point.
 *
 * \param [in,out] source A pointer to the source buffer. If the conversion
 * succeeds, this pointer will be updated to point to the byte just past the
 * end of the converted sequence.
 * \param sourceEnd A pointer just past the end of the source buffer.
 * \param [out] target The converted code
 * \param flags Whether the conversion is strict or lenient.
 *
 * \returns conversionOK on success
 *
 * \sa ConvertUTF8toUTF32
 */
static inline ConversionResult convertUTF8Sequence(const UTF8 **source,
                                                   const UTF8 *sourceEnd,
                                                   UTF32 *target,
                                                   ConversionFlags flags) {
  if (*source == sourceEnd)
    return sourceExhausted;
  unsigned size = getNumBytesForUTF8(**source);
  if ((ptrdiff_t)size > sourceEnd - *source)
    return sourceExhausted;
  return ConvertUTF8toUTF32(source, *source + size, &target, target + 1, flags);
}

/**
 * Returns true if a blob of text starts with a UTF-16 big or little endian byte
 * order mark.
 */
bool hasUTF16ByteOrderMark(ArrayRef<char> SrcBytes);

/**
 * Converts a stream of raw bytes assumed to be UTF16 into a UTF8 std::string.
 *
 * \param [in] SrcBytes A buffer of what is assumed to be UTF-16 encoded text.
 * \param [out] Out Converted UTF-8 is stored here on success.
 * \returns true on success
 */
bool convertUTF16ToUTF8String(ArrayRef<char> SrcBytes, std::string &Out);

/**
 * Converts a UTF-8 string into a UTF-16 string with native endianness.
 *
 * \returns true on success
 */
bool convertUTF8ToUTF16String(StringRef SrcUTF8,
                              SmallVectorImpl<UTF16> &DstUTF16);

//} /* end namespace llvm */

#endif//CONVERT_UTF_WRAPPER

namespace llvm {

#include "ConvertUTF.c"

bool ConvertUTF8toWide(unsigned WideCharWidth, llvm::StringRef Source,
                       char *&ResultPtr, const UTF8 *&ErrorPtr) {
  assert(WideCharWidth == 1 || WideCharWidth == 2 || WideCharWidth == 4);
  ConversionResult result = conversionOK;
  // Copy the character span over.
  if (WideCharWidth == 1) {
    const UTF8 *Pos = reinterpret_cast<const UTF8*>(Source.begin());
    if (!isLegalUTF8String(&Pos, reinterpret_cast<const UTF8*>(Source.end()))) {
      result = sourceIllegal;
      ErrorPtr = Pos;
    } else {
      memcpy(ResultPtr, Source.data(), Source.size());
      ResultPtr += Source.size();
    }
  } else if (WideCharWidth == 2) {
    const UTF8 *sourceStart = (const UTF8*)Source.data();
    // FIXME: Make the type of the result buffer correct instead of
    // using reinterpret_cast.
    UTF16 *targetStart = reinterpret_cast<UTF16*>(ResultPtr);
    ConversionFlags flags = strictConversion;
    result = ConvertUTF8toUTF16(
        &sourceStart, sourceStart + Source.size(),
        &targetStart, targetStart + 2*Source.size(), flags);
    if (result == conversionOK)
      ResultPtr = reinterpret_cast<char*>(targetStart);
    else
      ErrorPtr = sourceStart;
  } else if (WideCharWidth == 4) {
    const UTF8 *sourceStart = (const UTF8*)Source.data();
    // FIXME: Make the type of the result buffer correct instead of
    // using reinterpret_cast.
    UTF32 *targetStart = reinterpret_cast<UTF32*>(ResultPtr);
    ConversionFlags flags = strictConversion;
    result = ConvertUTF8toUTF32(
        &sourceStart, sourceStart + Source.size(),
        &targetStart, targetStart + 4*Source.size(), flags);
    if (result == conversionOK)
      ResultPtr = reinterpret_cast<char*>(targetStart);
    else
      ErrorPtr = sourceStart;
  }
  assert((result != targetExhausted)
         && "ConvertUTF8toUTFXX exhausted target buffer");
  return result == conversionOK;
}

bool ConvertCodePointToUTF8(unsigned Source, char *&ResultPtr) {
  const UTF32 *SourceStart = &Source;
  const UTF32 *SourceEnd = SourceStart + 1;
  UTF8 *TargetStart = reinterpret_cast<UTF8 *>(ResultPtr);
  UTF8 *TargetEnd = TargetStart + 4;
  ConversionResult CR = ConvertUTF32toUTF8(&SourceStart, SourceEnd,
                                           &TargetStart, TargetEnd,
                                           strictConversion);
  if (CR != conversionOK)
    return false;

  ResultPtr = reinterpret_cast<char*>(TargetStart);
  return true;
}

bool hasUTF16ByteOrderMark(ArrayRef<char> S) {
  return (S.size() >= 2 &&
          ((S[0] == '\xff' && S[1] == '\xfe') ||
           (S[0] == '\xfe' && S[1] == '\xff')));
}

bool convertUTF16ToUTF8String(ArrayRef<char> SrcBytes, std::string &Out) {
  assert(Out.empty());

  // Error out on an uneven byte count.
  if (SrcBytes.size() % 2)
    return false;

  // Avoid OOB by returning early on empty input.
  if (SrcBytes.empty())
    return true;

  const UTF16 *Src = reinterpret_cast<const UTF16 *>(SrcBytes.begin());
  const UTF16 *SrcEnd = reinterpret_cast<const UTF16 *>(SrcBytes.end());

  // Byteswap if necessary.
  std::vector<UTF16> ByteSwapped;
  if (Src[0] == UNI_UTF16_BYTE_ORDER_MARK_SWAPPED) {
    ByteSwapped.insert(ByteSwapped.end(), Src, SrcEnd);
    for (unsigned I = 0, E = ByteSwapped.size(); I != E; ++I)
      ByteSwapped[I] = llvm::sys::SwapByteOrder_16(ByteSwapped[I]);
    Src = &ByteSwapped[0];
    SrcEnd = &ByteSwapped[ByteSwapped.size() - 1] + 1;
  }

  // Skip the BOM for conversion.
  if (Src[0] == UNI_UTF16_BYTE_ORDER_MARK_NATIVE)
    Src++;

  // Just allocate enough space up front.  We'll shrink it later.  Allocate
  // enough that we can fit a null terminator without reallocating.
  Out.resize(SrcBytes.size() * UNI_MAX_UTF8_BYTES_PER_CODE_POINT + 1);
  UTF8 *Dst = reinterpret_cast<UTF8 *>(&Out[0]);
  UTF8 *DstEnd = Dst + Out.size();

  ConversionResult CR =
      ConvertUTF16toUTF8(&Src, SrcEnd, &Dst, DstEnd, strictConversion);
  assert(CR != targetExhausted);

  if (CR != conversionOK) {
    Out.clear();
    return false;
  }

  Out.resize(reinterpret_cast<char *>(Dst) - &Out[0]);
  Out.push_back(0);
  Out.pop_back();
  return true;
}

bool convertUTF8ToUTF16String(StringRef SrcUTF8,
                              SmallVectorImpl<UTF16> &DstUTF16) {
  assert(DstUTF16.empty());

  // Avoid OOB by returning early on empty input.
  if (SrcUTF8.empty()) {
    DstUTF16.push_back(0);
    DstUTF16.pop_back();
    return true;
  }

  const UTF8 *Src = reinterpret_cast<const UTF8 *>(SrcUTF8.begin());
  const UTF8 *SrcEnd = reinterpret_cast<const UTF8 *>(SrcUTF8.end());

  // Allocate the same number of UTF-16 code units as UTF-8 code units. Encoding
  // as UTF-16 should always require the same amount or less code units than the
  // UTF-8 encoding.  Allocate one extra byte for the null terminator though,
  // so that someone calling DstUTF16.data() gets a null terminated string.
  // We resize down later so we don't have to worry that this over allocates.
  DstUTF16.resize(SrcUTF8.size()+1);
  UTF16 *Dst = &DstUTF16[0];
  UTF16 *DstEnd = Dst + DstUTF16.size();

  ConversionResult CR =
      ConvertUTF8toUTF16(&Src, SrcEnd, &Dst, DstEnd, strictConversion);
  assert(CR != targetExhausted);

  if (CR != conversionOK) {
    DstUTF16.clear();
    return false;
  }

  DstUTF16.resize(Dst - &DstUTF16[0]);
  DstUTF16.push_back(0);
  DstUTF16.pop_back();
  return true;
}

} // end namespace llvm

