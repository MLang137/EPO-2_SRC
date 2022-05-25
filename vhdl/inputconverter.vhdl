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
    signal count : std_logic_vector(19 downto 0);
begin
	process(clk, reset)

	begin
		if(rising_edge(clk)) then
			if(reset = '1') then
				count <= (others => '0');
			elsif (mine_analog = '0') then
				count <= count + '1';
			elsif (mine_analog = '1') then
				count <= (others => '0');
			end if;
	        end if;
	end process;

	process(count, reset)

	begin
		if (reset = '1') then 
			mine_detected <= '0';
		elsif (to_integer(unsigned(count)) > 7700) then 
			mine_detected <= '1';
		elsif (to_integer(unsigned(count)) < 7700) then
			mine_detected <= '0';
		end if;
	end process;
end architecture behavioural;