#pragma once

#include <Core/Types.h>
#include <Core/PODArray.h>

void writeBits(UInt64 * dest, size_t bit_offset, UInt64 value)
{
    size_t mod = bit_offset / 64;
    dest[bit_offset / 64] |= value << mod;
    if (mod)
        dest[bit_offset / 64 + 1] |= value >> (64 - mod);
}


struct MarkInCompressedFile {
    size_t offset_in_compressed_file;
    size_t offset_in_decompressed_file;

    bool operator==(const MarkInCompressedFile & rhs)
    {
        return std::tie(offset_in_compressed_file, offset_in_decompressed_file)
               == std::tie(rhs.offset_in_compressed_file, rhs.offset_in_decompressed_file);
    }

    bool operator!=(const MarkInCompressedFile & rhs) { return operator!=(rhs); }

    std::tuple<size_t, size_t> asTuple()
    {
        return std::make_tuple(offset_in_compressed_file, offset_in_decompressed_file);
    }

    String toString()
    {
        return "(" + std::to_string(offset_in_compressed_file) + "," + std::to_string(offset_in_decompressed_file) + ")";
    }
};

class MarksInCompressedFile
{
public:
    MarksInCompressedFile(PODArray<MarkInCompressedFile> & plain_marks)
    : num_marks(plain_marks.size())
    {
        if (num_marks == 0)
        {
            return;
        }

        size_t packed_bits;
        blocks.reserve((num_marks + MARKS_PER_BLOCK - 1) / MARKS_PER_BLOCK);
        for (size_t i = 0; i < blocks.size(); ++i)
        {
            auto & block = blocks[i];
            block = BlockInfo{};
            size_t num_marks_in_block = std::min(MARKS_PER_BLOCK, num_marks - MARKS_PER_BLOCK * i);

            for (size_t idx = 0; idx < num_marks_in_block; ++idx)
            {
                auto & mark = plain_marks[i * MARKS_PER_BLOCK + idx];
                block.min_x = std::min(mark.offset_in_compressed_file, block.min_x);
                block.min_y = std::min(mark.offset_in_decompressed_file, block.min_y);
            }

            /// The best practice is to choose the minimum value of the difference
            block.bits_for_x = 8;
            block.bits_for_y = 8;
            packed_bits += block.bits_for_x * num_marks_in_block + block.bits_for_y * num_marks_in_block;
        }


        size_t bytes_offset_in_packed;
        packed.reserve((packed_bits + 63) / 64);
        for (size_t i = 0; i < blocks.size(); ++i)
        {
            auto & block = blocks[i];
            size_t num_marks_in_block = std::min(MARKS_PER_BLOCK, num_marks - MARKS_PER_BLOCK * i);
            block.byte_offset_in_packed_array = bytes_offset_in_packed;

            for (size_t idx = 0; idx < num_marks_in_block; ++idx)
            {
                auto & mark = plain_marks[i * MARKS_PER_BLOCK + idx];
                writeBits(packed.data(),
                          bytes_offset_in_packed + idx * (block.bits_for_y + block.bits_for_x),
                          mark.offset_in_compressed_file);
                writeBits(packed.data(),
                          bytes_offset_in_packed + idx * (block.bits_for_y + block.bits_for_x) + block.bits_for_x,
                          mark.offset_in_decompressed_file);
            }
        }
    }

    MarkInCompressedFile get(size_t idx);

private:
    struct BlockInfo
    {
        /// offset_in_compressed_file = x
        /// offset_in_decompressed_file = y
        size_t min_x;
        size_t min_y;

        size_t byte_offset_in_packed_array;

        size_t bits_for_x;
        size_t bits_for_y;
    };

    PODArray<BlockInfo> blocks;
    PODArray<UInt64> packed;
    size_t num_marks;

    static const size_t MARKS_PER_BLOCK = 256;

};










