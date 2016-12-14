// Copyright 2016 oneup

#include "Mapper.hpp"

#include <cassert>

#include "Machine.hpp"

namespace gblr {

size_t Mapper::Index(u16 addr) {
	size_t index = addr;
	if (number_ == kMapperMBC1 && (addr & 0x4000)) {
		index &= ~0x4000;
		index |= (bank_ << kBankShift);
	}

	assert(index < data_.size());
	return index;
}

void Mapper::Init(MapperNumber number, const u8 *data, size_t size) {
	number_ = number;
	data_ = std::vector<u8>(data, data + size);
	bank_ = 1;
}

void Mapper::Unload() {
	number_ = kMapperInvalid;
	data_.clear();
	bank_ = 1;
}

u8 Mapper::Read(u16 addr, bool) {
	size_t index = Index(addr);
    return data_[index];
}

void Mapper::Write(u16 addr, u8 val, bool debug) {
	size_t index = Index(addr);

    if (debug) { data_[index] = val; return; }

    switch (number_) {
		case kMapperNone: break;
		case kMapperMBC1:
			switch (addr & 0x6000) {
				case 0x0000:
					m_->Log("MBC1 RAM enable not implemented");
					break;
				case 0x2000:
					bank_ = val & 0x1f;
					if (!bank_) { bank_ = 1; }
					break;
				case 0x4000:
					m_->Log("MBC1 RAM bank select / ROM upper bank select not implemented");
					break;
				case 0x6000:
					m_->Log("MBC1 RAM/ROM mode select not implemented");
					break;
			}
			break;
		case kMapperInvalid:
			assert(0);
			break;
    }
}

}
