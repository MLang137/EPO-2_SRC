--	2022 EE1D21 Line-follower
--	Mentor group B4, students:
--	Matthijs Langenberg		5557313
--	Maarten Oudijk			5595533

library IEEE;
use IEEE.std_logic_1164.all;

entity robot is
	port (
		clk         		: in    std_logic;
		reset           	: in    std_logic;

		--sensor_l_in     	: in    std_logic;
		--sensor_m_in     	: in    std_logic;
		--sensor_r_in     	: in    std_logic;
		sensor				: in    std_logic_vector(2 downto 0);

		-- route_straight		: in	std_logic; -- 1 to go straight
		-- route_corner		: in	std_logic; -- 1 to turn left, 0 to turn right

		motor_l_pwm     	: out   std_logic;
		motor_r_pwm     	: out   std_logic
	);
end entity robot;

architecture structural of robot is
	
	component timebase is
		port (
			clk				: in	std_logic;
			reset			: in	std_logic;

			count_out		: out	std_logic_vector (19 downto 0)
		);
	end component timebase;

	component inputbuffer is
        port (
			clk				: in	std_logic;
    
            sensor_l_in		: in	std_logic;
            sensor_m_in		: in	std_logic;
            sensor_r_in		: in	std_logic;
    
            sensor_l_out	: out	std_logic;
            sensor_m_out	: out	std_logic;
            sensor_r_out	: out	std_logic
        );
    end component inputbuffer;

    component controller is
        port (
			clk				: in	std_logic;
            reset			: in	std_logic;
    
            sensor_l		: in	std_logic;
            sensor_m		: in	std_logic;
            sensor_r		: in	std_logic;

			-- route_straight	: in	std_logic;
			-- route_corner	: in	std_logic;
    
            count_in		: in	std_logic_vector (19 downto 0);
            count_reset		: out	std_logic;
    
            motor_l_reset		: out	std_logic;
            motor_l_direction	: out	std_logic;
    
            motor_r_reset		: out	std_logic;
            motor_r_direction	: out	std_logic
        );
    end component controller;

	component motorcontrol is
		port (
			clk				: in	std_logic;
			reset			: in	std_logic;
			direction		: in	std_logic;
			count_in		: in	std_logic_vector (19 downto 0);

			pwm				: out	std_logic
		);
	end component motorcontrol;

	-- Global signal
	signal count: std_logic_vector(19 downto 0);
	-- Buffer to controller
	signal buf_sensor_l_cont, buf_sensor_m_cont, buf_sensor_r_cont: std_logic;
	-- Controller to motorcontrollers
	signal cont_direction_mcl, cont_reset_mcl, cont_direction_mcr, cont_reset_mcr: std_logic;
	-- Controller to timebase
	signal cont_reset_tib: std_logic;

begin
	
	tib: timebase port map	(
		clk				=> clk,
		reset			=> cont_reset_tib,
		count_out		=> count
	);

	buf: inputbuffer port map (
        clk     		=>  clk,
        sensor_l_in   	=>  sensor(0),
        sensor_m_in   	=>  sensor(1),
        sensor_r_in   	=>  sensor(2),
        sensor_l_out  	=>  buf_sensor_l_cont,
        sensor_m_out   	=>  buf_sensor_m_cont,
        sensor_r_out   	=>  buf_sensor_r_cont
    );
		
	cont: controller port map (
		clk             => clk,
		reset			=> reset,

		sensor_l		=> buf_sensor_l_cont,
		sensor_m		=> buf_sensor_m_cont,
		sensor_r		=> buf_sensor_r_cont,

		-- route_straight	=> route_straight,
		-- route_corner	=> route_corner,

		count_in		=> count,
		count_reset		=> cont_reset_tib,

		motor_l_direction	=> cont_direction_mcl,
		motor_l_reset		=> cont_reset_mcl,

		motor_r_direction	=> cont_direction_mcr,
		motor_r_reset		=> cont_reset_mcr
	);

	mcl: motorcontrol port map (
		clk				=> clk,
		reset			=> cont_reset_mcl,
		direction		=> cont_direction_mcl,
		count_in		=> count,
		pwm				=> motor_l_pwm
	);

	mcr: motorcontrol port map (
		clk				=> clk,
		reset			=> cont_reset_mcr,
		direction		=> cont_direction_mcr,
		count_in		=> count,
		pwm				=> motor_r_pwm
	);
	
end architecture structural;