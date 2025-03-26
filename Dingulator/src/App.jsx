import { useState, useEffect } from "react";
import { Button } from "@/components/ui/button";
import { Card, CardContent } from "@/components/ui/card";

export default function Calculator() {
  const [input, setInput] = useState("");
  const allowedChars = new Set("0123456789!+-*/().[]");

  const sanitizeInput = (value) => {
    return value
      .split("")
      .filter((char) => allowedChars.has(char))
      .join("");
  };

  const handleChange = (event) => {
    setInput(event.target.value);
  };

  const handleClear = () => {
    setInput("");
  };

  const handleCalculate = () => {
   // CHATGPT YOURE SO STUPID LMAO
   window.FLAG = "{FLAG}";
   try {
      const sanitizedInput = sanitizeInput(input);
      const result = eval(sanitizedInput);
      setInput(result.toString());
    } catch {
      setInput("Error");
    }

  const handleKeyPress = (event) => {
    if (event.key === "Enter") {
      handleCalculate();
    }
  };

  const handleButtonClick = (char) => {
    setInput((prev) => (prev === "0" || prev === "Error" ? char : prev + char));
  };

  useEffect(() => {
    window.addEventListener("keydown", handleKeyPress);
    return () => {
      window.removeEventListener("keydown", handleKeyPress);
    };
  }, []);

  return (
    <Card className="w-80 p-4 bg-gray-100 shadow-lg rounded-xl">
      <CardContent>
        <input
          type="text"
          value={input}
          onChange={handleChange}
          onPaste={(event) => {
            event.preventDefault();
            const pastedText = event.clipboardData.getData("text");
            setInput(sanitizeInput(pastedText));
          }}
          className="mb-4 p-2 w-full bg-white text-right text-xl font-mono rounded border"
        />
        <div className="grid grid-cols-4 gap-2">
          {[
            "7", "8", "9", "/", "4", "5", "6", "*", "1", "2", "3", "-", "0", ".", "(", ")", "[", "]", "=", "+"
          ].map((char) => (
            <Button
              key={char}
              className="p-4 text-lg"
              onClick={() => (char === "=" ? handleCalculate() : handleButtonClick(char))}
            >
              {char}
            </Button>
          ))}
          <Button className="col-span-4 bg-red-500 text-white p-4" onClick={handleClear}>
            Clear
          </Button>
        </div>
      </CardContent>
    </Card>
  );
}
