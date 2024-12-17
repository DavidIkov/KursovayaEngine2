#pragma once

template<size_t BoolsAmount>
class BoolsTableClass {
public:
    unsigned char Bools[BoolsAmount / 8 + ((BoolsAmount % 8 == 0) ? 0 : 1)];
public:
    constexpr BoolsTableClass() noexcept { for (size_t i = 0; i < sizeof(Bools); i++) Bools[i] = 0; }

    constexpr BoolsTableClass(const BoolsTableClass<BoolsAmount>& toCopy) noexcept { for (size_t i = 0; i < sizeof(Bools); i++) Bools[i] = toCopy.Bools[i]; }
    constexpr BoolsTableClass(const BoolsTableClass<BoolsAmount>&& toMove) noexcept { for (size_t i = 0; i < sizeof(Bools); i++) Bools[i] = toMove.Bools[i]; }
    constexpr BoolsTableClass<BoolsAmount>& operator=(const BoolsTableClass<BoolsAmount>& toCopy) noexcept { new(this) BoolsTableClass<BoolsAmount>(toCopy); }
    constexpr BoolsTableClass<BoolsAmount>& operator=(const BoolsTableClass<BoolsAmount>&& toMove) noexcept { new(this) BoolsTableClass<BoolsAmount>(std::move(toMove)); }

    constexpr bool operator[](size_t ind) const noexcept {
        unsigned char curByte = Bools[ind / 8];
        unsigned int bitInd = ind % 8;
        unsigned char mask = 1 << bitInd;
        unsigned char curBool = curByte & mask;
        return curBool != 0;
    }
    constexpr bool Get(size_t ind) const noexcept { return operator[](ind); }
    constexpr void Set(size_t ind, bool val) noexcept {
		unsigned char curByte = Bools[ind / 8];
        unsigned int bitInd = ind % 8;
        if (val)
            Bools[ind / 8] = curByte | (1 << bitInd);
        else Bools[ind / 8] = curByte & ~(1 << bitInd);
	}
};
