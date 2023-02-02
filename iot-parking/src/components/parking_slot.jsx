import React, { useEffect } from 'react'

let instancesCount = 0;

export default function Slot() {
    const [plurality, setPlurality] = React.useState(0);
    const [slotData, setSlotData] = React.useState(0);

    useEffect(() => {
        setInterval(() => {
            fetch('http://localhost:8192/').then((res) => res.json().then((data) => setSlotData(data.slots)));

        }, 2.5*1000);
        instancesCount += 1;
        setPlurality(instancesCount);
        return () => {
            instancesCount -= 1;
            setPlurality(instancesCount);
        }
    }, [])

    return (
        <div className="slot-parent">
            <div className={"slot " + (slotData[plurality] === "1" ? 'unavailable' : 'available')}>
                <span>Slot {plurality}</span>
            </div>
        </div>
    )
}