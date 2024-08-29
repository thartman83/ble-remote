import { MouseEvent } from "react";
import "./button.css";

export type ButtonClickEvent = MouseEvent<HTMLButtonElement>;

export enum ButtonType {
  primary,
  secondary,
};

export type ButtonProps = {
  label: string,
  clickHandler: (e: ButtonClickEvent) => void,
  buttonType: ButtonType,
};

const Button = ({label, clickHandler, buttonType}: ButtonProps) => {
  return (
    <button className={"btn " + (buttonType === ButtonType.primary ?
      "btn-primary" : "btn-secondary")}
      onClick={clickHandler}>{label}</button>
  );
};

export default Button;
