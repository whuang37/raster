//=======================================================================
// Copyright 2015-2107
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//

#pragma once

#include <blink/raster/gdal_includes.h>

#include <memory> //shared_ptr

namespace blink
{
  namespace raster
  {
    template<class> // template argument for AccessType is now ignored
    struct block
    {
      using iterator = char*;
  
      block() = default;
     
      void reset(GDALRasterBand* band, int major_row, int major_col)
      {
        GDALRasterBlock* block = band->GetLockedBlockRef(major_col, major_row);
        if (block == nullptr) {
          assert(false);
          throw("trying to open inaccessible GDALRasterBlock");
        }
        auto deleter = [](GDALRasterBlock* b) {b->DropLock(); };
        m_block.reset(block, deleter);
      }

      void reset(int major_row, int major_col)
      {
        GDALRasterBand* band = m_block->GetBand();
        reset(band, major_row, major_col);
      }

      void reset()
      {
        m_block.reset();
      }

      int block_rows()
      {
        return m_block->GetYSize();
      }

      int block_cols()
      {
        return m_block->GetXSize();
      }

      int major_row()
      {
        return m_block->GetYOff();
      }

      int major_col()
      {
        return m_block->GetXOff();
      }

      iterator get_iterator(int minor_row, int minor_col, unsigned char stride)
      {
        char* block_start = static_cast<char*>(m_block->GetDataRef());
        return block_start + stride * (minor_row * block_cols() + minor_col);
      }

      iterator get_null_iterator()
      {
        return nullptr;
      }
  
      void mark_dirty()
      {
        m_block->MarkDirty();
      }

    private:
      std::shared_ptr<GDALRasterBlock> m_block;
    };
  }
}