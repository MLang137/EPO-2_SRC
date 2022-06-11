--	2022 EE1D21 Line-follower
--	Mentor group B4, students:
--	Matthijs Langenberg		5557313
--	Maarten Oudijk			5595533

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity motorcontrol is
	port (	clk		: in	std_logic;
		reset		: in	std_logic;
		direction	: in	std_logic;
		count_in	: in	std_logic_vector(19 downto 0);
		pwm		: out	std_logic
	);
end entity motorcontrol;

architecture behavioural of motorcontrol is
	
	type motor_state is (motor_off, motor_on);
	signal state, new_state: motor_state;

begin
	-- Switch states
	reg: process(clk)
	
	begin
		if rising_edge(clk) then
			if reset = '1' then
				state <= motor_off;
			else
				state <= new_state;
			end if;
		end if;
	end process;

	-- Control output
	ctrl: process(clk, count_in)

		constant one_ms: unsigned := to_unsigned(50000, 20);
		constant two_ms: unsigned := to_unsigned(100000, 20);

	begin
		if rising_edge(clk) then
			case state is
				when motor_off =>
					pwm <= '0';
					if reset = '0' then new_state <= motor_on;
					else new_state <= motor_off;
					end if;
				when motor_on =>
					pwm <= '1';
					-- wait 1 ms
					if direction = '0' and unsigned(count_in) > one_ms then
						pwm <= '0';
					-- wait 2 ms
					elsif direction = '1' and unsigned(count_in) > two_ms then
						pwm <= '0';
					end if;
			end case;
		end if;
	end process;

	
end architecture behavioural;