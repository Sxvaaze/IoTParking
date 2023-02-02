import React from 'react';
import '../styles/header.css';
import { Link } from 'react-router-dom';
export default function Header() {
    return (
        <div className="header">
            <div className="pages">
                <span className="header-text"><Link to="/">Parking Slots</Link></span>
                <span className="header-text"><Link to="/audit">Parking Audit</Link></span>
            </div>
        </div>
    )
}