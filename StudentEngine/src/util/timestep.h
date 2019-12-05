#pragma once

class TimeStep {
private:
    float m_time;
	float m_totalTime;
	int m_frameCount;
public:

    TimeStep(float initial = 0.0, float totalTime = 0.0f, int frameCount = 0)
        : m_time(initial), m_totalTime(totalTime), m_frameCount(frameCount) {
    }

	operator float() const { return m_time; }

    inline float GetSeconds() const { return m_time / 1000.0f; }
	inline float GetMills() const { return m_time; }

	inline float TotalTime() const { return m_totalTime; }
	inline int FrameCount() const { return m_frameCount; }
};