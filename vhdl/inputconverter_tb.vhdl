library IEEE;
use IEEE.std_logic_1164.all;

entity converter_tb is
end entity converter_tb;

architecture structural of converter_tb is
	component input_converter is
	port(
		clk, reset  : in std_logic;
		mine_analog : in std_logic;
		mine_detected : out std_logic);
	end component input_converter;

signal clk, reset, mine_detected, mine_analog1, mine_analog2, mine_analog, activate : std_logic;

begin
	conv: input_converter port map(clk => clk, reset => reset, mine_detected => mine_detected, mine_analog => mine_analog);

	clk <= '0' after 0 ns, '1' after 10 ns when clk /= '1' else '0' after 10 ns;

	reset <= '1' after 0 ns, '0' after 100 ns;

	mine_analog1 <=  '0' after 0 ns, '1' after 38 us when mine_analog1 /= '1' else '0' after 38 us;
	mine_analog2 <= '0' after 0 ns, '1' after 50 us when mine_analog2 /= '1' else '0' after 26 us;
	activate <= '0' after 0 ns, '1' after 300 us, '0' after 400 us;
process(clk)
begin 
	if(activate = '0') then
		mine_analog <= mine_analog1;
	else mine_analog <= mine_analog2;
	end if;
end process;

		
--mine_analog <= mine_analog1 after 0 ns, mine_analog2 after 300 us;
	 

end architecture;