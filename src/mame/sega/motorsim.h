
/*
	John Bennett 2026

	Motor position simulator(s)
	Integrate speeds from game
	Sets limit switches
	Pretty simple
*/

class motor
{
public:


	motor(int min_adc, int max_adc, float end2endtime, bool invert)
	{
		//TODO: Take out some of these game-specific numbers and generalise more
		const float FULLSPEED = 7.0f;
		const float SAMPLETIME = 60.0f;
		m_speedScaler = -(256.0f / SAMPLETIME) / (FULLSPEED * end2endtime);
		if (invert)
			m_speedScaler = -m_speedScaler;
		m_lowadclimit = min_adc;
		m_highadclimit = max_adc;
	}

	void reset()
	{
		m_position = centre;
	}
	int run(int motorcode, bool brake)
	{

		int speed = 0.0f;
		if (brake == false) 
		{
			speed = motorcode;
		}

		m_position += (float)speed * m_speedScaler;

		// Limit switches.
		// Game PCB may stop motion when set (often in self-test)
		if (m_position >= m_highadclimit)
		{
			m_limhigh = true;
		}
		else
		{
			m_limhigh = false;
		}

		if (m_position <= m_lowadclimit)
		{
			m_limlow = true;
		}
		else
		{
			m_limlow = false;
		}

		//TODO: remove sega values
		if ((m_position > 126.0f) && (m_position < 130.0f))
		{
			m_centre = true;
		}
		else
		{
			m_centre = false;
		}

		//8-bit clamps
		if (m_position > 255.0f)
		{
			m_position = 255.0f;
		}
		else if (m_position < 0.0f)
		{
			m_position = 0.0f;
		}

		return (int)m_position;
	}


	// Mode 2
	// For use when a drive PCB 'controller' is not properly emulated
	// i.e. when we only have a position command and the game doesn't read back ADCs
	// The max speed is input, the actual speed will vary with the distance from target
	// Better than going full-blast and dead-stopping at the target
	// 'Proportional only control'

	int run_open_loop(int target, int speed)
	{
		float target_f = 1.0f * (float)target;

		float error = target_f - m_position;
		error *= m_speedScaler;
		error *= (float)speed * 1.0f / 7.0f; //TODO - remove the scaling

		m_position -= error;

		// Limit switches.
		// Game PCB will stop motion itself as it winds towards target
	/*	if (m_position >= m_highadclimit)
		{
			m_limhigh = true;
		}
		else
		{
			m_limhigh = false;
		}

		if (m_position <= m_lowadclimit)
		{
			m_limlow = true;
		}
		else
		{
			m_limlow = false;
		}*/

		// 8-bit clamp
		if (m_position > 255.0f)
			m_position = 255.0f;
		else if (m_position < 0.0f)
			m_position = 0.0f;

		return (int)m_position;
	}


	bool getLowerLimit(void)
	{
		return m_limlow;
	}
	bool getUpperLimit(void)
	{
		return m_limhigh;
	}
	bool getCentre(void)
	{
		return m_centre;
	}
private:


	float m_position = 128.0f;
	bool m_limlow = false;
	bool m_limhigh = false;
	bool m_centre = false;
	float centre = 128.0f;

	float m_speedScaler = 0;
	float m_lowadclimit = 86;
	float m_highadclimit = 171;

};

