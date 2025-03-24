import Chart from "@/components/Chart";
import Image from "next/image";

export default function Home() {
  return (
    <div className="space-y-4">
      <Chart />
      {/* <Chart name="Accel Y"/>
      <Chart name="Accel Z"/>  */}
    </div>
  );
}
