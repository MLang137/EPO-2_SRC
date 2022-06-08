library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.all;

entity input_converter is
	port(clk   : in std_logic;
	     reset : in std_logic;
	
		mine_analog : in std_logic;
		mine_detected : out std_logic 
	);
end entity input_converter;

architecture behavioural of input_converter is
    signal count, count2 : std_logic_vector(19 downto 0);
signal wester: std_logic;
	 type motor_controller_state is (counter_on, counter_off);
    signal state, new_state : motor_controller_state;
begin
	process(clk, reset)

	begin
		if(rising_edge(clk)) then
			if(reset = '1') then
				count <= (others => '0');
			elsif (mine_analog = '0') then
				count <= count + '1';
			elsif (mine_analog = '1') then
				count <= (others => '0') ;
			end if;
	        end if;
	end process;

	process(count, reset)

	begin
		if (reset = '1') then 
			wester <= '0';
		elsif (to_integer(unsigned(count)) > 2200) then 
			wester <= '1';
		elsif (to_integer(unsigned(count)) < 2200) then
			wester <= '0';
		end if;
	end process;
	process(clk, reset)
    	begin
       		 if(rising_edge(clk)) then
            		if(reset = '1') then
               		 state <= counter_off;
           		 else
                	state <= new_state;
           		 end if;
        	end if;
    	end process;
	process(state, wester, clk)
        begin
            
		if( rising_edge(clk)) then
			case state is 
				when counter_off =>
					count2 <= (others => '0');
					mine_detected <= '0';
				if (wester = '1')then 
					new_state <= counter_on;
				else new_state <= counter_off;
				end if;
				when counter_on=>
 					mine_detected <= '1';
					count2 <= count2 + '1';
				if (to_integer(unsigned(count2)) > 10000 and wester ='0') then
					new_state <= counter_off;
				else new_state <= counter_on;
				end if;
			end case;
		end if;
	end process;
end architecture behavioural;