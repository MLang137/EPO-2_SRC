library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity controller is
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
end entity controller;

architecture behavioural of controller is

	type controller_state is ( 	con_reset,
					con_gentle_left,
					con_gentle_right,
					con_left,
					con_right,
					con_forward);

signal state, new_state: controller_state;
begin
	process (clk)
	begin
		if (rising_edge (clk)) then 
			if (reset = '1') then
				state <= con_reset;
			else
				state <= new_state;
			end if;
		end if;
	end process;

	process(count_in,sensor_l,sensor_m, sensor_r)
	begin
		case state is
			when con_reset =>
				motor_l_reset <= '1';
				motor_r_reset <= '1';
				if(sensor_l ='0' and sensor_r ='1' and sensor_m = '0') then	--gently left
					new_state <= con_gentle_left;
				elsif(sensor_l ='1' and sensor_r ='0' and sensor_m = '0') then	--gently right
					new_state <= con_gentle_right;
				elsif(sensor_l ='0' and sensor_r ='1' and sensor_m = '1') then -- left
					new_state <= con_left;
				elsif(sensor_l ='1' and sensor_r ='0' and sensor_m = '1') then	--right
					new_state <= con_right;
				else 								--forward
					new_state <= con_forward;	
				end if;
			when con_gentle_left =>
				motor_r_reset <= '0';
				motor_r_direction <= '0';
				if(unsigned(count_in) > 1000000) then
					new_state <= con_reset;
				end if;
			when con_gentle_right =>
				motor_l_reset <= '0';
				motor_l_direction <= '1';
				if(unsigned(count_in) > 1000000) then
					new_state <= con_reset;
				end if;
			when con_left =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '0';
				motor_r_direction <= '0';
				if(unsigned(count_in) > 1000000) then
					new_state <= con_reset;
				end if;
			when con_right =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '1';
				motor_r_direction <= '1';
				if(unsigned(count_in) > 1000000) then
					new_state <= con_reset;
				end if;
			when con_forward =>
				motor_l_reset <= '0';
				motor_r_reset <= '0';
				motor_l_direction <= '1';
				motor_r_direction <= '0';
				if(unsigned(count_in) > 1000000) then
					new_state <= con_reset;
				end if;
		end case;
	end process;
	process (reset, count_in)
	begin
		if(reset='1') then
			count_reset <= '1';
		else
			count_reset <= '0';
			if(unsigned(count_in) > 1000000) then
				count_reset <= '1';
			end if;
		end if;
	end process;

end architecture behavioural;