#include <iostream>
#include <limits>

namespace Decimal {

template <uint16_t SIZE>
class Decimal2048_t {
 public:
  enum ValueBit { kZero, kUnit };
  enum ValueSign { kPlus, kMinus };
  Decimal2048_t() = default;
  Decimal2048_t(Decimal2048_t const &other) { *this = other; }
  ~Decimal2048_t() = default;

  auto operator=(Decimal2048_t const &other) -> Decimal2048_t & {
    if (this != &other) {
      for (int i = 0; i < kSumCellsBits; ++i) {
        m_bits[i] = other.m_bits[i];
      }
    }
    return *this;
  }

  /*=== Возвращает новый Decimal2048_t со двигом влево ===*/
  auto operator<<(uint16_t shift) const -> Decimal2048_t {
    Decimal2048_t d;
    for (int16_t start_bit = shift, bit = 0, elder_bit = ElderBit();
         bit <= elder_bit; ++bit, ++start_bit) {
      if (start_bit < (kSumCellsBits - 1) * kSumBitsInCell) {
        d.SetBitDecimal_(start_bit, GetBitDecimal_(bit));
      }
    }
    return d;
  }

  /*=== Сдвигает Decimal2048_t влево ===*/
  auto operator<<=(uint16_t shift) -> void { *this = (*this) << shift; }

  /*=== Возвращает новый Decimal2048_t со двигом вправо ===*/
  auto operator>>(uint16_t shift) const -> Decimal2048_t {
    Decimal2048_t d;
    for (uint16_t start_bit = 0, bit = shift, elder_bit = ElderBit();
         bit <= elder_bit; ++bit, ++start_bit) {
      d.SetBitDecimal_(start_bit, GetBitDecimal_(bit));
    }
    return d;
  }

  /*=== Сдвигает Decimal2048_t вправо ===*/
  auto operator>>=(uint16_t shift) -> void { *this = (*this) >> shift; }

  /*=== Возвращает новый Decimal2048_t, результат умножения  ===*/
  auto operator*(Decimal2048_t const &other) const -> Decimal2048_t {
    Decimal2048_t d(*this);
    d.MulDecimal(other);
    return d;
  }

  /*=== Возвращает новый Decimal2048_t, результат умножения  ===*/
  template <class TypeName>
  auto operator*(TypeName num) const -> Decimal2048_t {
    Decimal2048_t d(*this);
    d.MulDecimal(num);
    return d;
  }

  /*=== Умножает на произвольный Decimal2048_t  ===*/
  auto operator*=(Decimal2048_t const &other) -> void { MulDecimal(other); }

  /*=== Возвращает новый Decimal2048_t, результат сложения  ===*/
  auto operator+(Decimal2048_t const &other) const -> Decimal2048_t {
    Decimal2048_t d(*this);
    d.AddDecimal(other);
    return d;
  }

  template <class TypeName>
  auto operator+(TypeName num) const -> Decimal2048_t {
    Decimal2048_t d(*this);
    d.AddValue(num);
    return d;
  }

  /*=== Добавляет произвольный Decimal2048_t  ===*/
  auto operator+=(Decimal2048_t const &other) -> void { AddDecimal(other); }

  template <class TypeName>
  auto operator+=(TypeName num) -> void {
    AddValue(num);
  }

  auto operator~() const -> Decimal2048_t {
    Decimal2048_t d;
    for (int i = 0; i < SIZE; ++i) {
      d.m_bits[i] = ~m_bits[i];
    }
    d.m_bits[SIZE] = m_bits[SIZE];  // знак
    return d;
  }

  auto SubDecimal(Decimal2048_t const &other) -> void {
    //
  }

  /*=== Добавляет произвольный Decimal2048_t  ===*/
  auto AddDecimal(Decimal2048_t const &other) -> void {
    bool sign_d_1 = m_bits[SIZE];
    bool sign_d_2 = m_bits[SIZE];

    // если оба положительных или оба отрицательных

    if ((sign_d_1 == kSign && sign_d_2 == kSign) ||
        (sign_d_1 != kSign && sign_d_2 != kSign)) {
      AddDecimal_(other);
    } else {
      if (sign_d_1 == kSign) {
        //   if (*this > other) {
        //     // *this - other
        //   } else {
        //     // other - this
        //   }
        // } else {
        //   if (*this > other) {
        //     //other - this
        //   } else {
        //   // this - other
        //   }
        // }
      }
    }

    // bool unit{ValueBit::kZero};
    // uint16_t elder_bit =
    //     ElderBit() > other.ElderBit() ? ElderBit() : other.ElderBit();
    // uint16_t bit{0};
    // while (bit <= elder_bit) {
    //   bool bit_decimal_1 = GetBitDecimal_(bit);
    //   bool bit_decimal_2 = other.GetBitDecimal_(bit);
    //   // если оба бита = 1
    //   if (bit_decimal_1 && bit_decimal_2) {
    //     if (unit == ValueBit::kUnit) {
    //       SetBitDecimal_(bit, ValueBit::kUnit);
    //     } else {
    //       SetBitDecimal_(bit, ValueBit::kZero);
    //       unit = ValueBit::kUnit;
    //     }
    //     // если один из двух бит = 1
    //   } else if (bit_decimal_1 || bit_decimal_2) {
    //     if (unit == ValueBit::kUnit) {
    //       SetBitDecimal_(bit, ValueBit::kZero);
    //     } else {
    //       SetBitDecimal_(bit, ValueBit::kUnit);
    //     }
    //     // оба бита = 0 и была запомнена единица
    //   } else if (unit == ValueBit::kUnit) {
    //     SetBitDecimal_(bit, ValueBit::kUnit);
    //     unit = ValueBit::kZero;
    //   }
    //   ++bit;
    // }
    // // если единица перешла в старшйи разряд
    // if (unit) {
    //   SetBitDecimal_(bit, ValueBit::kUnit);
    // }
  }

  /*=== Добавляет произвольный Decimal2048_t  ===*/
  auto AddDecimal_(Decimal2048_t const &other) -> void {
    bool unit{ValueBit::kZero};
    uint16_t elder_bit =
        ElderBit() > other.ElderBit() ? ElderBit() : other.ElderBit();
    uint16_t bit{0};
    while (bit <= elder_bit) {
      bool bit_decimal_1 = GetBitDecimal_(bit);
      bool bit_decimal_2 = other.GetBitDecimal_(bit);
      // если оба бита = 1
      if (bit_decimal_1 && bit_decimal_2) {
        if (unit == ValueBit::kUnit) {
          SetBitDecimal_(bit, ValueBit::kUnit);
        } else {
          SetBitDecimal_(bit, ValueBit::kZero);
          unit = ValueBit::kUnit;
        }
        // если один из двух бит = 1
      } else if (bit_decimal_1 || bit_decimal_2) {
        if (unit == ValueBit::kUnit) {
          SetBitDecimal_(bit, ValueBit::kZero);
        } else {
          SetBitDecimal_(bit, ValueBit::kUnit);
        }
        // оба бита = 0 и была запомнена единица
      } else if (unit == ValueBit::kUnit) {
        SetBitDecimal_(bit, ValueBit::kUnit);
        unit = ValueBit::kZero;
      }
      ++bit;
    }
    // если единица перешла в старшйи разряд
    if (unit) {
      SetBitDecimal_(bit, ValueBit::kUnit);
    }
  }

  auto SubDecimal_(Decimal2048_t const &other) -> void {
    //
  }

  /*=== Умножает на произвольный Decimal2048_t  ===*/
  auto MulDecimal(Decimal2048_t const &other) -> void {
    Decimal2048_t result;
    for (uint16_t bit = 0, shift = 0, elder_bit = ElderBit(); bit <= elder_bit;
         ++bit, ++shift) {
      if (GetBitDecimal_(bit)) {
        result.AddDecimal(other << shift);
        Decimal2048_t gg(other << shift);
      }
    }
    *this = result;
  }

  /*=== Умножает на произвольное число  ===*/
  template <class TypeName>
  auto MulDecimal(TypeName num) -> void {
    MulDecimal(ValueToDecimal(num));
  }

  /*=== Устанавливает в  Decimal2048_t значение другого произвольного
   * Decimal2048_t ===*/
  auto InstallDecimal(Decimal2048_t const &other) -> void {
    for (uint8_t cell = 0; cell < kSumCellsBits; ++cell) {
      m_bits[cell] = other.m_bits[cell];
    }
  }

  /*=== Устанавливает в  Decimal2048_t значение произвольного числа ===*/
  template <class TypeName>
  auto InstallValue(TypeName num) -> void {
    CheckType_<TypeName>();  // проверка на корреткный тип
    ZeroDecimal_();          // зануляем децимал
    auto sum_bits = sizeof(TypeName) * 8;  // не превышает 64 бит
    for (auto bit = 0; bit < sum_bits; ++bit) {
      if (bit < 32) {
        m_bits[0] |= (GetBitValue_<TypeName>(num, bit) << bit);
      } else {
        m_bits[1] |= (GetBitValue_<TypeName>(num, bit) << bit);
      }
    }
  }

  /*=== Возвращает Decimal2048_t, от произвольного числа ===*/
  template <class TypeName>
  auto ValueToDecimal(TypeName num) const -> Decimal2048_t {
    Decimal2048_t d;
    d.InstallValue(num);
    return d;
  }

  /*=== Добавляет произвольное число к Decimal2048_t ===*/
  template <class TypeName>
  auto AddValue(TypeName num) -> void {
    AddDecimal(ValueToDecimal(num));
  }

  /*=== Печатает Decimal2048_t, каждые 4 байта на новой строке ===*/
  auto PrintDecimal() const -> std::string {
    std::string view_decimal{};
    for (int bit = kSumCellsBits * kSumBitsInCell - 1; bit >= 0; --bit) {
      //
      view_decimal += std::to_string(GetBitDecimal_(bit));
      if (bit % 4 == 0 && bit != 0) {
        view_decimal += " ";
      }
      if (bit % 32 == 0 && bit != 0) {
        view_decimal += "\n";
      }
      //
    }
    return view_decimal;
  }

 protected:
  /*=== Зануляет Decimal2048_t ===*/
  auto ZeroDecimal_() -> void {
    for (auto &i : m_bits) {
      i = 0;
    }
  }

  /*=== Возвращает значение бита, у Decimal2048_t ===*/
  auto GetBitDecimal_(uint16_t bit) const -> bool {
    return (m_bits[bit / kSumBitsInCell] >> bit) & 1;
  }

  /*=== Меняет значение бита у Decimal2048_t на 'value_bit' ===*/
  auto SetBitDecimal_(uint16_t bit, bool value_bit) -> void {
    m_bits[bit / kSumBitsInCell] = m_bits[bit / kSumBitsInCell] & ~(1 << bit);
    m_bits[bit / kSumBitsInCell] |= (value_bit << bit);
  }

  /*=== Возвращает значение бита, у произвольного числа ===*/
  template <class TypeName>
  auto GetBitValue_(TypeName num, uint16_t bit) const -> bool {
    auto bytes = sizeof(TypeName);
    bool res_bit{};
    if (bytes == sizeof(uint8_t)) {
      res_bit = ((uint8_t)num >> bit) & 1;
    } else if (bytes == sizeof(uint16_t)) {
      res_bit = ((uint16_t)num >> bit) & 1;
    } else if (bytes == sizeof(uint32_t)) {
      res_bit = ((uint32_t)num >> bit) & 1;
    } else {
      res_bit = ((uint64_t)num >> bit) & 1;
    }
    return res_bit;
  }

  /*=== Проверяет является ли шаблон  обрабатываемым типом ===*/
  template <class TypeName>
  auto CheckType_() const -> void {
    if (!IsPossibleType_<TypeName>()) {
      throw std::invalid_argument(
          "Incorrect type, possible type to install: uint8_t, uint16_t, "
          "uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, unsigned "
          "long , long");
    }
  }

  /*=== Возваращает 'True', если шаблон является обрабатываемым типом ===*/
  template <class TypeName>
  auto IsPossibleType_() const -> bool {
    bool pissible_type = (std::is_same<TypeName, int8_t>::value ||
                          std::is_same<TypeName, int16_t>::value ||
                          std::is_same<TypeName, int32_t>::value ||
                          std::is_same<TypeName, int64_t>::value ||
                          std::is_same<TypeName, uint8_t>::value ||
                          std::is_same<TypeName, uint16_t>::value ||
                          std::is_same<TypeName, uint32_t>::value ||
                          std::is_same<TypeName, uint64_t>::value ||
                          std::is_same<TypeName, long>::value ||
                          std::is_same<TypeName, unsigned long>::value);

    return pissible_type;
  }

 public:
  /*--- Возвращает позиция старшего бита ---*/
  auto ElderBit() const -> uint16_t {
    uint16_t bit = (kSumCellsBits - 1) * kSumBitsInCell - 1;
    while (bit != 0 && !GetBitDecimal_(bit)) {
      --bit;
    }
    return bit;
  }

 private:
  // 1 ячейка идет под знак
  const uint8_t kSumCellsBits = SIZE + 1;
  const uint32_t kSign = 0x80000000;
  const uint8_t kSumBitsInCell = 32;
  uint32_t m_bits[SIZE + 1]{};
};
};  // namespace Decimal
