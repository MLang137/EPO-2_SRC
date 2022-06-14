--	2022 EE1D21 Line-follower
--	Mentor group B4, students:
--	Matthijs Langenberg		5557313
--	Maarten Oudijk			5595533

library IEEE;
use IEEE.std_logic_1164.all;

entity controller_tb is
end entity controller_tb;

architecture structural of controller_tb is

	component timebase is
		port (	clk			: in	std_logic;
			reset			: in	std_logic;

			count_out		: out	std_logic_vector (19 downto 0)
		);
	end component timebase;

    component controller is
        port (	clk			: in	std_logic;
            reset			: in	std_logic;
    
            sensor_l		: in	std_logic;
            sensor_m		: in	std_logic;
            sensor_r		: in	std_logic;
    
            count_in		: in	std_logic_vector (19 downto 0);
            count_reset		: out	std_logic;
    
            motor_l_reset		: out	std_logic;
            motor_l_direction	: out	std_logic;
    
            motor_r_reset		: out	std_logic;
            motor_r_direction	: out	std_logic;

		route_straight		: in	std_logic; -- 1 to go straight
		route_corner		: in	std_logic; -- 1 to turn left, 0 to turn right
		mine_detected		: in	std_logic; -- mine detected digital signal
		stop					: in	std_logic; -- completely stop the robot permanently
		maneuver_complete : out std_logic; -- to get next instruction from pc

		uart_write			: out std_logic;
		uart_read			: out std_logic
        );
    end component controller;

	signal	clk, reset, sensor_l, sensor_m, sensor_r		                        : std_logic;
	signal	count_in				                                                : std_logic_vector (19 downto 0);
	signal	count_reset, motor_l_reset, motor_l_direction, motor_r_reset, motor_r_direction      : std_logic;
	signal	route_straight, route_corner, mine_detected, stop, maneuver_complete, uart_write, uart_read: std_logic;
    
	signal	sensors				: std_logic_vector(2 downto 0);

begin

	tib: timebase port map	(clk	=> clk,
					reset			=> count_reset,
					count_out		=> count_in
				);
					
	eut: controller port map (
                    clk             => clk,
                    reset			=> reset,

                    sensor_l		=> sensor_l,
                    sensor_m		=> sensor_m,
                    sensor_r		=> sensor_r,

                    count_in		=> count_in,
                    count_reset		=> count_reset,

                    motor_l_reset		=> motor_l_reset,
                    motor_l_direction	=> motor_l_direction,

                    motor_r_reset		=> motor_r_reset,
                    motor_r_direction	=> motor_r_direction,

			route_straight => route_straight,
			route_corner => route_corner,
			mine_detected => mine_detected,
			stop => stop,
			maneuver_complete => maneuver_complete,
			uart_write => uart_write,
			uart_read => uart_read
				);

	-- 20 ns = 1/50 MHz
	clk			<=	'0' after 0 ns,
					'1' after 10 ns when clk /= '1' else '0' after 10 ns;

	reset			<=	'1' after 0 ns,
					'0' after 60 ns;
					-- '1' after 120 ms,
					-- '0' after 130 ms;

    sensors 		<=	"000" after 0 ns,   -- bbb
                    "101" after 70 ms,  -- wbw
                    "000" after 110 ms, -- wbw
                    "001" after 150 ms, -- wbw
                    "010" after 190 ms, -- bbb
                    "101" after 230 ms, -- bbw
                    "000" after 250 ms, -- bwb
                    "101" after 252 ms; -- wbw

    route_straight	<= '0' after 70 ms;
    route_corner	<= '1' after 70 ms; -- received 'p' to turn right

    sensor_l		<= 	sensors(2);
    sensor_m		<= 	sensors(1);
    sensor_r		<= 	sensors(0);

end architecture structural;