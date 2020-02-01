#pragma once


class GameTimer
{
public:
	GameTimer() = default;
	~GameTimer() = default;

	void Update(double deltaTime);

	void Start();
	void Pause();
	void Reset();
	void Stop();
	bool HasStarted() { return started && !paused; }

	double timePassed{ 0 };

private:
	bool started { false };
	bool paused { false };
};

inline void GameTimer::Update(double deltaTime)
{
	if(started && !paused)
	{
		timePassed += deltaTime;
	}
}

inline void GameTimer::Start()
{
	started = true;
	paused = false;
}

inline void GameTimer::Pause()
{
	paused = true;
}

inline void GameTimer::Reset()
{
	timePassed = 0;
}

inline void GameTimer::Stop()
{
	started = false;
	paused = false;
	timePassed = 0;
}
