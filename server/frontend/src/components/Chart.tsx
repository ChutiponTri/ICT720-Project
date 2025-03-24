"use client"

import * as React from "react"
import { Area, AreaChart, CartesianGrid, XAxis } from "recharts"
import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from "@/components/ui/card"
import {
  ChartConfig,
  ChartContainer,
  ChartLegend,
  ChartLegendContent,
  ChartTooltip,
  ChartTooltipContent,
} from "@/components/ui/chart"
import MQTT from "./MQTT"

const initData = [
  { date: new Date().toISOString(), accelX: 0, accelY: 0, accelZ: 0, gyroX: 0, gyroY: 0, gyroZ: 0 },
];

const chartConfig = {
  visitors: {
    label: "Visitors",
  },
  x_axis: {
    label: "X",
    color: "hsl(var(--chart-1))",
  },
  y_axis: {
    label: "Y",
    color: "hsl(var(--chart-2))",
  },
  z_axis: {
    label: "Z",
    color: "hsl(var(--chart-3))",
  },
} satisfies ChartConfig

type ChartProps = {
  name: string; // Define the type of the 'name' prop
};

function Chart() {
  const [chartData, setChartData] = React.useState(initData)

  // Handle any data filtering based on your needs
  const filteredData = chartData // Here, filtering can be based on any condition you prefer

  // Function to generate random data for both desktop and mobile
  React.useEffect(() => {
    const dataCallback = (data: { axm: number[], aym: number[], azm: number[], gxm: number[], gym: number[], gzm: number[] }) => {
      const newTimestamp = new Date().toISOString();
      const newChartData = data.axm.map((value, index) => ({
        date: newTimestamp,
        accelX: value,
        accelY: data.aym[index],
        accelZ: data.azm[index],
        gyroX: data.gxm[index],
        gyroY: data.gym[index],
        gyroZ: data.gzm[index],
      }));
      setChartData((prevData) => {
        const updatedData = [...prevData, ...newChartData];
        
        // Ensure the total length does not exceed 200 by dropping the oldest entries
        return updatedData.length > 100 ? updatedData.slice(updatedData.length - 200) : updatedData;
      });
    };

  // Create a new MQTT client and pass the dataCallback to handle incoming data
  const mqtt = new MQTT("broker.emqx.io", 8083, "ton/server/m5", dataCallback);

  return () => {
    // Clean up MQTT client
    
  };
  }, []);

  const chartMetrics = [
    { key: "accelX", color: "--color-x_axis", label: "Acceleration X" },
    { key: "accelY", color: "--color-y_axis", label: "Acceleration Y" },
    { key: "accelZ", color: "--color-z_axis", label: "Acceleration Z" },
    { key: "gyroX", color: "--color-x_axis", label: "Gyroscope X" },
    { key: "gyroY", color: "--color-y_axis", label: "Gyroscope Y" },
    { key: "gyroZ", color: "--color-z_axis", label: "Gyroscope Z" },
  ];

  return (
    <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
      {chartMetrics.map(({ key, color, label }) => (
        <Card key={key}>
          <CardHeader className="border-b py-5">
            <CardTitle>{label}</CardTitle>
          </CardHeader>
          <CardContent className="px-2 pt-4 sm:px-6 sm:pt-6">
            <ChartContainer config={chartConfig} className="h-[250px] w-full">
              <AreaChart data={chartData}>
                <defs>
                  <linearGradient id={`fill${key}`} x1="0" y1="0" x2="0" y2="1">
                    <stop offset="5%" stopColor={`var(${color})`} stopOpacity={0.8} />
                    <stop offset="95%" stopColor={`var(${color})`} stopOpacity={0.1} />
                  </linearGradient>
                </defs>
                <CartesianGrid vertical={false} />
                <XAxis dataKey="date" tickLine={false} axisLine={false} tickMargin={8} />
                <ChartTooltip cursor={false} content={<ChartTooltipContent />} />
                <Area dataKey={key} type="natural" fill={`url(#fill${key})`} stroke={`var(${color})`} />
                <ChartLegend content={<ChartLegendContent />} />
              </AreaChart>
            </ChartContainer>
          </CardContent>
        </Card>
      ))}
    </div>
  );
}

export default Chart;
