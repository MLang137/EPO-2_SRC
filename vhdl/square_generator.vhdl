library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.all;

entity wave_generator is
	port(clk   : in std_logic;
	     reset : in std_logic;
	     wave_out : out std_logic 
	);
end entity wave_generator;

architecture behavioural of wave_generator is
        signal count, new_count :std_logic_vector(10 downto 0);
	type controller_state is (out_high, out_low, reset_state, count_reset_state);
	signal state, new_state :  controller_state;
	signal out_or_high : std_logic;
begin 
process(clk, reset)
    	begin
       		 if(rising_edge(clk)) then
            		if(reset = '1') then
               		 state <= reset_state;
			 else
                	state <= new_state;
           		 end if;
        	end if;
    	end process;
process(count)
    begin
        new_count <= count + '1';
    end process;

process(clk, count, state)
        begin
            if( rising_edge(clk)) then
			case state is 
				when out_high =>
					wave_out <= '1';
					out_or_high <= '1';
				if (to_integer(unsigned(count)) > 1880 ) then
					new_state <= count_reset_state;
				else new_state <= out_high;
				count <= new_count;
				end if;	
				
				when reset_state =>
					out_or_high <= '1';
					count <= (others => '0');
					wave_out <= '0';
					new_state <= out_high;

				when out_low =>
 					wave_out <= '0';
					out_or_high <= '0';
				if (to_integer(unsigned(count)) > 1880 ) then
					new_state <= count_reset_state;
				else new_state <= out_low;
				count <= new_count;
				end if;
				
				when count_reset_state =>
					count <= (others => '0');
					if (out_or_high = '1')then
					new_state <= out_low;
					else
					new_state <= out_high;
					end if;
				end case;
		end if;
	end process;
end architecture behavioural;
