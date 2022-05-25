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

signal clk, reset, mine_detected, mine_analog1, mine_analog2, mine_analog: std_logic;

begin
	conv: input_converter port map(clk => clk, reset => reset, mine_detected => mine_detected, mine_analog => mine_analog);

	clk <= '0' after 0 ns, '1' after 10 ns when clk /= '1' else '0' after 10 ns;

	reset <= '1' after 0 ns, '0' after 25 ns;

	mine_analog1 <=  '0' after 0 ns, '1' after 38 us when mine_analog /= '1' else '0' after 38 us;
	mine_analog2 <= '1' after 1 ns, '0' after 380 us;
	mine_analog <= mine_analog1 and mine_analog2;

end architecture;