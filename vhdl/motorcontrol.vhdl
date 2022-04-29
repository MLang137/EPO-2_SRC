library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity motorcontrol is
	port ( 	clk : in std_logic;
		reset : in std_logic;
		direction : in std_logic;
		count_in : in std_logic_vector (19 downto 0);  

		pwm : out std_logic
	);
end entity motorcontrol;


architecture behavioural of motorcontrol is

	type motorcontrol_state is ( 	pwm_reset,
					pwm_on,
					pwm_low);

signal state, new_state: motorcontrol_state;
begin

process (clk)
	begin
		if (rising_edge (clk)) then 
			if (reset = '1') then
				state <= pwm_reset;
			else
				state <= new_state;
			end if;
		end if;
		end process;


process (state, direction, count_in)
	begin
		case state is
			when pwm_reset =>
				pwm <= '0';
				new_state <= pwm_on;
			when pwm_on =>
				pwm <= '1';
				if(direction = '0' and unsigned(count_in) > 50000) then
					new_state          <=pwm_low;
				elsif (direction = '1' and unsigned(count_in) > 100000) then
					new_state          <=pwm_low;
				end if;
			when pwm_low =>
  				pwm <= '0';
		end case;
end process;

end architecture behavioural;


