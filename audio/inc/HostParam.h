#pragma once

#include <atomic>

#include "JuceHeader.h"
#include "Param.h"

template<typename _par>
class HostParam : public AudioProcessorParameter, public Param::Listener {
public:
    HostParam(_par &p) : param(p) {
        param.addListener(this);
    }

    ~HostParam() {
        param.removeListener(this);
    }

    float getValue() const override {
        return engineToHost(param.getUI());
    }

    void setValue(float newValue) override {
        jassert(newValue >= 0.f && newValue <= 1.f);
        param.setHost(hostToEngine(newValue));
    }

    virtual float getDefaultValue() const override {
        return engineToHost(param.getDefault());
    }

    virtual String getName(int maximumStringLength) const override {
        return param.name().substring(0,maximumStringLength);
    }

    virtual String getLabel() const override {
        return param.unit();
    }

    //! \todo implement this
    virtual float getValueForText(const String& text) const override {
        return text.getFloatValue();
    }


    virtual void paramUIChanged() override {
        // this will (unnecessarily) call setValue
        setValueNotifyingHost(engineToHost(param.getUI()));
    }

protected:
    float engineToHost(float engineVal) const {
        jassert(engineVal >= param.getMin() && engineVal <= param.getMax());
        return (engineVal - param.getMin()) / (param.getMax() - param.getMin());
    }
    float hostToEngine(float hostVal) const {
        jassert(hostVal >= 0.f && hostVal <= 1.f);
        return (param.getMin() + hostVal*(param.getMax() - param.getMin()));
    }

    _par &param;
};