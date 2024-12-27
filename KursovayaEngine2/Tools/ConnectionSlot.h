#pragma once
class ConnectionSlotC {
protected:
	ConnectionSlotC* OtherSlot = nullptr;
public:
    ConnectionSlotC() noexcept {}
    ConnectionSlotC(const ConnectionSlotC&) = delete;
    ConnectionSlotC(ConnectionSlotC&& toMove) noexcept:
        OtherSlot(toMove.OtherSlot) {
        toMove.OtherSlot = nullptr;
        if (OtherSlot != nullptr) OtherSlot->OtherSlot = this;
    }
    virtual ~ConnectionSlotC() {
        if (OtherSlot != nullptr) {
            OtherSlot->OtherSlot = nullptr;
            OtherSlot->~ConnectionSlotC();
            OtherSlot = nullptr;
        }
    }
    void Connect(ConnectionSlotC& otherCon) {
        if (OtherSlot != nullptr) Disconnect();
        if (otherCon.OtherSlot != nullptr) Disconnect();
        OtherSlot = &otherCon;
        otherCon.OtherSlot = this;
    }
    void Disconnect() { this->~ConnectionSlotC(); }
};

