#pragma once

class Custom {
  public:
    static inline int numCopyCtorCalls{0};
    static inline int numMoveCtorCalls{0};

    Custom() : d_{} {}

    explicit Custom(double d) : d_(d) {}

    Custom(const Custom& c) : d_(c.d_) { ++numCopyCtorCalls; }

    Custom(Custom&& c) noexcept(false) : d_(c.d_) { ++numMoveCtorCalls; }

    friend auto operator==(const Custom&, const Custom&) noexcept -> bool = default;

  private:
    double d_;
};

class Custom2 {
  public:
    Custom2() : d_{} {}

    explicit Custom2(double d) : d_(d) {}

    Custom2(const Custom2& c) = delete;

    Custom2(Custom2&& c) = default;

    friend auto operator==(const Custom2&, const Custom2&) noexcept -> bool = default;

    ~Custom2() {}

  private:
    double d_;
};
