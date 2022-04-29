library IEEE;
use IEEE.std_logic_1164.all;

entity robot is
	port (  clk             : in    std_logic;
		reset           : in    std_logic;

		sensor_l_in     : in    std_logic;
		sensor_m_in     : in    std_logic;
		sensor_r_in     : in    std_logic;

		motor_l_pwm     : out   std_logic;
		motor_r_pwm     : out   std_logic
	);
end entity robot;

architecture structural of robot is
	component timebase is
 		port (  clk 		: in std_logic;
                        reset   	: in std_logic;
                        count_out       : out std_logic_vector (19 downto 0)
		);
	end component timebase;
	component inputbuffer is
		port (	clk		: in	std_logic;
			sensor_l_in	: in	std_logic;
			sensor_m_in	: in	std_logic;
			sensor_r_in	: in	std_logic;
	
			sensor_l_out	: out	std_logic;
			sensor_m_out	: out	std_logic;
			sensor_r_out	: out	std_logic
		);
	end component inputbuffer;
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
			motor_r_direction	: out	std_logic
		);
	end component controller;
	component motorcontrol is
		port ( 	clk : in std_logic;
			reset : in std_logic;
			direction : in std_logic;
			count_in : in std_logic_vector (19 downto 0);  
			pwm : out std_logic 
		);
	end component motorcontrol;

signal timereset, senl, senr, senm, motlres, motldir, motrres, motrdir: std_logic;
signal counting: std_logic_vector(19 downto 0);

begin
t1:	timebase port map( 	clk=>clk, reset=>timereset, count_out=> counting);
i1:	inputbuffer port map(	clk=>clk,
				sensor_l_in=>sensor_l_in, sensor_m_in=>sensor_m_in, sensor_r_in=>sensor_r_in, 
				sensor_l_out=>senl,sensor_r_out=>senr,sensor_m_out=>senm);
c1:	controller port map(	clk=>clk, reset=>reset, 
				sensor_l=>senl, sensor_m=>senm, sensor_r=>senr, 
				count_in=>counting, count_reset=>timereset,
				motor_l_reset=>motlres,motor_l_direction=>motldir, motor_r_reset=> motrres,motor_r_direction=> motrdir);
	
mr:	motorcontrol port map(	clk=>clk, reset=>motrres, direction=> motrdir, count_in=>counting,pwm=>motor_r_pwm);
ml:	motorcontrol port map(	clk=>clk, reset=>motlres, direction=> motldir, count_in=>counting,pwm=>motor_l_pwm);
end architecture structural;