import React from 'react';
import Slot from './parking_slot';
import '../styles/home.css';

export default function Home() {
    return (
        <div className="home-container">
            <div className="slot-container">
                <Slot/>
                <Slot/>
                <Slot/>
            </div>
        </div>
    )
}